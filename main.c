#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "operations/operations.h"
#include "img_processing/processing.h"
#include "text_to_char/segmentation.h"
#include "img_processing/rotation.h"
#include "neural_network/OCR/network.h"

#define UNUSED(x) (void) x

typedef struct UserInterface
{
	GtkWindow* window;
	GtkButton* start_button;
	GtkButton* right1;
	GtkButton* right10;
	GtkButton* right90;
	GtkButton* left10;
	GtkImage* image_space;
	GtkFileChooserButton* choosefile;
	GtkLabel* rotate_label;
	GtkTextView* text_space;
	GtkButton* train_button;
	GtkButton* save_button;
	SDL_Surface* image_surface;

} UserInterface;


//Start the OCR
gboolean on_start(GtkButton *button, gpointer user_data)
{
	UNUSED(button);
	UserInterface* interface = user_data;
	//Desactivate all the button until in other image is loaded
	gtk_widget_set_sensitive(GTK_WIDGET(interface->start_button), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(interface->right1), FALSE); 
    gtk_widget_set_sensitive(GTK_WIDGET(interface->right10), FALSE); 
    gtk_widget_set_sensitive(GTK_WIDGET(interface->right90), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(interface->left10), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(interface->save_button), TRUE);

	SDL_Surface* image = interface->image_surface;

	//Greyscale, noise reduction, blackandwhite
	treatment(image);

	//Segmentation per line in red
	linesegm(image);

	//Segmentation per character in green
	charsegm(image);

	//Extraction of each character
	//Calling of the neural network
	//Writting in the file
	segmentation(image);

	//Free memory and close SDL
	SDL_FreeSurface(image);
	SDL_Quit();

	//Print the text in "text.txt" in the window
	gchar *buff;
	gsize size;
	if(g_file_get_contents("text.txt", &buff, &size, NULL))
	{
		GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW
					(interface->text_space));
		gtk_text_buffer_set_text (buffer, buff, size);
		gtk_text_view_set_buffer (interface->text_space, buffer);
	}

	return TRUE;
}


//Load the file choose with the FileChooserButton
gboolean load_file(GtkButton *button, gpointer user_data)
{
	UserInterface* interface = user_data;
	//Get the path of the choosen image and load it
	char* path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(button));
	interface->image_surface = load_image(path);

	//Activate all the buttons
	gtk_widget_set_sensitive(GTK_WIDGET(interface->start_button), TRUE); 
	gtk_widget_set_sensitive(GTK_WIDGET(interface->right1), TRUE); 
    gtk_widget_set_sensitive(GTK_WIDGET(interface->right10), TRUE); 
    gtk_widget_set_sensitive(GTK_WIDGET(interface->right90), TRUE); 
    gtk_widget_set_sensitive(GTK_WIDGET(interface->left10), TRUE);

	//Save the image in BMP to display it
	SDL_SaveBMP(interface->image_surface, "tmp.bmp");
	gtk_image_set_from_file (interface->image_space, "tmp.bmp");
	return FALSE;
}


//Save the modify text in the file "text.txt"
gboolean on_save(GtkButton *button, gpointer user_data)
{
	UNUSED(button);
	UserInterface* interface = user_data;

	//Get the text of the GtkTextView
	GtkTextIter start, end;
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(interface->text_space);
	gchar *text;
	gtk_text_buffer_get_bounds (buffer, &start, &end);
	text = gtk_text_buffer_get_text (buffer, &start, &end, FALSE);

	//Overwrite the file "text.txt"
	FILE *file = fopen("text.txt", "w+");
	fputs(text, file);
	fclose(file);
	return FALSE;
}


//Rotation 1° to right
gboolean rotate1(GtkButton *button, gpointer user_data)
{
	UNUSED(button);
	UserInterface* interface = user_data;
	SDL_Surface* image = rotate(interface->image_surface, 1);
	interface->image_surface = image;
	SDL_SaveBMP(interface->image_surface, "tmp.bmp");
	gtk_image_set_from_file (interface->image_space, "tmp.bmp");
	return FALSE;
}
//Rotation 10° to right
gboolean rotate10(GtkButton *button, gpointer user_data)
{
	UNUSED(button);
	UserInterface* interface = user_data;
	SDL_Surface* image = rotate(interface->image_surface, 10);
	interface->image_surface = image;
	SDL_SaveBMP(interface->image_surface, "tmp.bmp");
	gtk_image_set_from_file (interface->image_space, "tmp.bmp");
	return FALSE;
}
//Rotation 90° to right
gboolean rotate90(GtkButton *button, gpointer user_data)
{
	UNUSED(button);
	UserInterface* interface = user_data;
	SDL_Surface* image = rotate(interface->image_surface, 90);
	interface->image_surface = image;
	SDL_SaveBMP(interface->image_surface, "tmp.bmp");
	gtk_image_set_from_file (interface->image_space, "tmp.bmp");
	return FALSE;
}
//Rotation 10° to left
gboolean rotateleft(GtkButton *button, gpointer user_data)
{
	UNUSED(button);
	UserInterface* interface = user_data;
	SDL_Surface* image = rotate(interface->image_surface, -10);
	interface->image_surface = image;
	SDL_SaveBMP(interface->image_surface, "tmp.bmp");
	gtk_image_set_from_file (interface->image_space, "tmp.bmp");
	return FALSE;
}


//Train the neural network
gboolean trainNet(GtkButton *button, gpointer user_data)
{
	UNUSED(button);
	UserInterface* interface = user_data;
	GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
	GtkWidget *dialog = gtk_message_dialog_new_with_markup(interface->window,
				flags, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE,
				"The window will be closed to load the new neural network.");
	gtk_window_set_title(GTK_WINDOW(dialog), "Warning");

	gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_widget_destroy (dialog);

	net_train();
	return FALSE;
}




//The main fonction which does everything
int main ()
{
	// Initializes GTK.
	gtk_init(NULL, NULL);

	// Constructs a GtkBuilder instance.
	GtkBuilder* builder = gtk_builder_new ();

	// Loads the UI description.
	// (Exits if an error occurs.)
	GError* error = NULL;
	if (gtk_builder_add_from_file(builder, "ocr2.glade", &error) == 0)
	{
		g_printerr("Error loading file: %s\n", error->message);
		g_clear_error(&error);
		return 1;
	}

	init_sdl();

	// Gets the widgets.
	GtkWindow* window = GTK_WINDOW(gtk_builder_get_object
							(builder, "org.gtk.ocr"));
	GtkButton* start_button = GTK_BUTTON(gtk_builder_get_object
							(builder, "start_button"));
	GtkButton* right1 = GTK_BUTTON(gtk_builder_get_object(builder, "right1"));
    GtkButton* right10 = GTK_BUTTON(gtk_builder_get_object(builder,"right10"));
    GtkButton* right90 = GTK_BUTTON(gtk_builder_get_object(builder,"right90"));
    GtkButton* left10 = GTK_BUTTON(gtk_builder_get_object(builder, "left10"));
	GtkLabel* rotate_label = GTK_LABEL(gtk_builder_get_object
							(builder, "rotate_label"));
    GtkFileChooserButton* choosefile = GTK_FILE_CHOOSER_BUTTON
							(gtk_builder_get_object(builder, "choosefile"));
    GtkImage* image_space = GTK_IMAGE(gtk_builder_get_object
							(builder, "image_space"));
	GtkTextView* text_space = GTK_TEXT_VIEW(gtk_builder_get_object
							(builder, "text_space"));
	GtkButton* train_button = GTK_BUTTON(gtk_builder_get_object
							(builder, "train_button"));
	GtkButton* save_button = GTK_BUTTON(gtk_builder_get_object
							(builder, "save_button"));


	//Initialise all the widget in the struct
	UserInterface interface =
	{
		.window = window,
		.start_button = start_button,
		.right1 = right1,
		.right10 = right10,
		.right90 = right90,
		.left10 = left10,
		.rotate_label = rotate_label,
		.choosefile = choosefile,
		.image_space = image_space,
		.text_space = text_space,
		.image_surface = NULL,
		.train_button = train_button,
		.save_button = save_button,
	};

	//if no images have been choosen, disable the other buttons
	if(gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(interface.choosefile))
								== NULL)
	{
		gtk_widget_set_sensitive(GTK_WIDGET(interface.start_button), FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(interface.right1), FALSE); 
		gtk_widget_set_sensitive(GTK_WIDGET(interface.right10), FALSE); 
		gtk_widget_set_sensitive(GTK_WIDGET(interface.right90), FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(interface.left10), FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(interface.save_button), FALSE);
	}

	// Connects event handlers.
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(start_button, "clicked", G_CALLBACK(on_start),&interface);
	g_signal_connect(right1, "clicked", G_CALLBACK(rotate1), &interface);
	g_signal_connect(right10, "clicked", G_CALLBACK(rotate10), &interface);
	g_signal_connect(right90, "clicked", G_CALLBACK(rotate90), &interface);
	g_signal_connect(left10, "clicked", G_CALLBACK(rotateleft), &interface);
	g_signal_connect(choosefile, "file_set", G_CALLBACK(load_file),&interface);
	g_signal_connect(train_button, "clicked",G_CALLBACK(trainNet), &interface);
	g_signal_connect(save_button, "clicked", G_CALLBACK(on_save), &interface);

    // Runs the main loop.
    gtk_main();

    // Exits.
    return 0;

}