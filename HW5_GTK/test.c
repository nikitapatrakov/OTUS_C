#include <gtk/gtk.h>
#include <stdio.h>

static void click_button(GtkButton *btn) {
  printf("%s\n", gtk_button_get_label(btn));
}

static void
activate (GtkApplication *app,
          gpointer        user_data)
{
  GtkWidget *window = gtk_application_window_new(GTK_APPLICATION(app));
  gtk_window_set_title(GTK_WINDOW(window), "Nikita Tester");
  gtk_window_set_default_size(GTK_WINDOW(window), 250,200);

  GtkWidget* btn1 = gtk_button_new_with_label("Anna");
  gtk_widget_set_halign(btn1, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(btn1, GTK_ALIGN_CENTER);

  g_signal_connect(btn1, "clicked", G_CALLBACK(click_button), NULL);
  gtk_window_set_child(GTK_WINDOW(window), btn1);
  gtk_window_present(GTK_WINDOW(window));
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