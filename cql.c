#include <my_global.h>
#include <mysql.h>
#include <gtk/gtk.h>
#include <string.h>
#include "sql_util.h"

int main(int argc, char *argv[])
{
    GtkBuilder      *builder;
    GtkWidget       *window;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "layout.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    gtk_builder_connect_signals(builder, NULL);

    g_object_unref(builder);

    gtk_widget_show(window);
    gtk_main();
    return 0;
}

// called when window is closed
void on_window_main_destroy()
{
    gtk_main_quit();
}

// called when run button is clicked
void on_run_button_clicked(GtkWidget *button, gpointer user_data)
{
    GtkWidget *toplevel = gtk_widget_get_toplevel (button);
    GtkWidget *child = gtk_bin_get_child(GTK_BIN(toplevel));
    GList *children = gtk_container_get_children(GTK_CONTAINER(child));
    for (GList *l = children; l != NULL; l = l->next)
    {
        const char *name = gtk_widget_get_name(l->data);
        if (strcmp(name, "pane_2") == 0)
        {
            child = l->data;
            break;
        }
    }
    children = gtk_container_get_children(GTK_CONTAINER(child));
    for (GList *l = children; l != NULL; l = l->next)
    {
        const char *name = gtk_widget_get_name(l->data);
        if (strcmp(name, "main_text_view") == 0)
        {
            child = l->data;
            break;
        }
    }
    GtkTextIter start, end;
    GtkTextBuffer *buffer = gtk_text_view_get_buffer ((GtkTextView*)child);
    gchar *text;

    gtk_text_buffer_get_bounds (buffer, &start, &end);
    text = gtk_text_buffer_get_text (buffer, &start, &end, FALSE);
    /* execute query */
    query("localhost", "test_user", "test_pass", "testdb", text);
    g_free(text);
}