# cql

Compile with:

gcc sql_util.c cql.c -o cql \`mysql_config --cflags --libs\` \`pkg-config --libs --cflags gtk+-3.0\` -export-dynamic
