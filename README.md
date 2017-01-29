# cql

Compile with gcc sql-cmd.c -o sql-cmd \`mysql_config --cflags --libs\`

For gui:

gcc -o sql-gui sql-gui.c -Wall `\pkg-config --cflags --libs gtk+-3.0\` -export-dynamic