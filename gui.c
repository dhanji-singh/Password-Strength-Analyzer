#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <gtk/gtk.h>

// Forward declarations
int analyzePassword(const char *password);
const char *getFeedback(int strength, int length);

// AppWidgets structure definition
typedef struct {
    GtkWidget *password_entry;
    GtkWidget *result_label;
    GtkWidget *strength_bar;
    GtkWidget *feedback_label;
} AppWidgets;

// Global CSS provider
GtkCssProvider *css_provider = NULL;

void on_password_changed(GtkEditable *editable, gpointer user_data) {
    AppWidgets *widgets = (AppWidgets *)user_data;
    const char *pwd = gtk_entry_get_text(GTK_ENTRY(widgets->password_entry));
    int strength = analyzePassword(pwd);
    int len = strlen(pwd);

    // Update result label
    if (len == 0) {
        gtk_label_set_text(GTK_LABEL(widgets->result_label), "");
    }
    else if (strength == 0) {
        gtk_label_set_markup(GTK_LABEL(widgets->result_label), 
            "<span foreground='#d32f2f' font_size='16000' weight='bold'>WEAK PASSWORD</span>");
    }
    else if (strength == 1) {
        gtk_label_set_markup(GTK_LABEL(widgets->result_label), 
            "<span foreground='#f57c00' font_size='16000' weight='bold'>MODERATE PASSWORD</span>");
    }
    else {
        gtk_label_set_markup(GTK_LABEL(widgets->result_label), 
            "<span foreground='#388e3c' font_size='16000' weight='bold'>STRONG PASSWORD</span>");
    }

    // Update strength bar
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(widgets->strength_bar), 
        (double)(strength + 1) / 3.0);

    // Update feedback
    gtk_label_set_text(GTK_LABEL(widgets->feedback_label), getFeedback(strength, len));
}

static void activate(GtkApplication *app, gpointer user_data) {
    // Allocate memory for widgets structure (FIXED: was using stack variable)
    AppWidgets *widgets = g_malloc(sizeof(AppWidgets));
    
    // Create main window
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Password Strength Analyzer");
    gtk_window_set_default_size(GTK_WINDOW(window), 450, 350);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    
    // Create main container
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
    gtk_widget_set_margin_top(main_box, 20);
    gtk_widget_set_margin_bottom(main_box, 20);
    gtk_widget_set_margin_start(main_box, 20);
    gtk_widget_set_margin_end(main_box, 20);
    gtk_window_set_child(GTK_WINDOW(window), main_box);

    // Initialize CSS provider (FIXED: moved to initialization, not in callback)
    css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(css_provider, 
        "progressbar { min-height: 20px; } "
        "progressbar trough { background-color: #ffebee; } "
        "progressbar progress { background-color: #d32f2f; }",
        -1, NULL);
    
    GtkStyleContext *context = gtk_widget_get_style_context(main_box);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(css_provider), 
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    // Create title label
    GtkWidget *title_label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title_label), 
        "<span font_size='18000' weight='bold'>Password Strength Analyzer</span>");
    gtk_box_append(GTK_BOX(main_box), title_label);

    // Create password entry label
    GtkWidget *entry_label = gtk_label_new("Enter Password:");
    gtk_widget_set_halign(entry_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(main_box), entry_label);

    // Create password entry
    widgets->password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(widgets->password_entry), FALSE);
    gtk_entry_set_placeholder_text(GTK_ENTRY(widgets->password_entry), "Type your password...");
    gtk_box_append(GTK_BOX(main_box), widgets->password_entry);

    // Create result label
    widgets->result_label = gtk_label_new("");
    gtk_box_append(GTK_BOX(main_box), widgets->result_label);

    // Create strength bar
    widgets->strength_bar = gtk_progress_bar_new();
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(widgets->strength_bar), 0.0);
    gtk_box_append(GTK_BOX(main_box), widgets->strength_bar);

    // Create feedback label
    widgets->feedback_label = gtk_label_new("Enter a password to analyze");
    gtk_widget_set_wrap_mode(widgets->feedback_label, GTK_WRAP_WORD);
    gtk_box_append(GTK_BOX(main_box), widgets->feedback_label);

    // Connect signal handler (FIXED: now passes heap-allocated widgets)
    g_signal_connect(widgets->password_entry, "changed", 
        G_CALLBACK(on_password_changed), widgets);

    // Connect cleanup handler
    g_signal_connect(window, "destroy", 
        G_CALLBACK(g_free), widgets);

    // Show all widgets
    gtk_widget_show(window);
}
