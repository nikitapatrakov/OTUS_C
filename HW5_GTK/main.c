#include <gtk/gtk.h>

GListModel *create_list_model_cb(gpointer item, gpointer user_data) {
   GListStore *list_date = g_list_store_new(G_TYPE_STRING);
   return G_LIST_MODEL(list_date);
}
static void
activate (GtkApplication *app,
          gpointer        user_data)
{
  GtkWidget *window; 
  GListStore *list = g_list_store_new(G_TYPE_STRING);
  GtkTreeListModel *model = gtk_free_list_model_new(
      G_LIST_MODEL(model),
      FALSE,
      FALSE,
      create_list_model_cb,
      NULL,
      NULL
  );
  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Window");
  gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);

  

  gtk_window_present (GTK_WINDOW (window));
}

int
main (int    argc,
      char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}