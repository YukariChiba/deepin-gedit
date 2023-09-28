/*
 * gedit-view-commands.c
 * This file is part of gedit
 *
 * Copyright (C) 1998, 1999 Alex Roberts, Evan Lawrence
 * Copyright (C) 2000, 2001 Chema Celorio, Paolo Maggi
 * Copyright (C) 2002-2005 Paolo Maggi
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include "gedit-commands.h"
#include "gedit-commands-private.h"

#include <gtk/gtk.h>

#include "gedit-debug.h"
#include "gedit-window.h"
#include "gedit-highlight-mode-dialog.h"
#include "gedit-highlight-mode-selector.h"

void
_gedit_cmd_view_focus_active (GSimpleAction *action,
                              GVariant      *state,
                              gpointer       user_data)
{
	GeditView *active_view;
	GeditWindow *window = GEDIT_WINDOW (user_data);

	gedit_debug (DEBUG_COMMANDS);

	active_view = gedit_window_get_active_view (window);

	if (active_view)
	{
		gtk_widget_grab_focus (GTK_WIDGET (active_view));
	}
}

void
_gedit_cmd_view_toggle_side_panel (GSimpleAction *action,
                                   GVariant      *state,
                                   gpointer       user_data)
{
	GeditWindow *window = GEDIT_WINDOW (user_data);
	GtkWidget *panel;
	gboolean visible;

	gedit_debug (DEBUG_COMMANDS);

	panel = gedit_window_get_side_panel (window);

	visible = g_variant_get_boolean (state);
	gtk_widget_set_visible (panel, visible);

	if (visible)
	{
		gtk_widget_grab_focus (panel);
	}

	g_simple_action_set_state (action, state);
}

void
_gedit_cmd_view_toggle_bottom_panel (GSimpleAction *action,
                                     GVariant      *state,
                                     gpointer       user_data)
{
	GeditWindow *window = GEDIT_WINDOW (user_data);
	GtkWidget *panel;
	gboolean visible;

	gedit_debug (DEBUG_COMMANDS);

	panel = gedit_window_get_bottom_panel (window);

	visible = g_variant_get_boolean (state);
	gtk_widget_set_visible (panel, visible);

	if (visible)
	{
		gtk_widget_grab_focus (panel);
	}

	g_simple_action_set_state (action, state);
}

void
_gedit_cmd_view_toggle_fullscreen_mode (GSimpleAction *action,
                                        GVariant      *state,
                                        gpointer       user_data)
{
	GeditWindow *window = GEDIT_WINDOW (user_data);

	gedit_debug (DEBUG_COMMANDS);

	if (g_variant_get_boolean (state))
	{
		_gedit_window_fullscreen (window);
	}
	else
	{
		_gedit_window_unfullscreen (window);
	}
}

void
_gedit_cmd_view_leave_fullscreen_mode (GSimpleAction *action,
                                       GVariant      *parameter,
                                       gpointer       user_data)
{
	_gedit_window_unfullscreen (GEDIT_WINDOW (user_data));
}

static void
language_selected_cb (GeditHighlightModeSelector *selector,
		      GtkSourceLanguage          *language,
		      GeditWindow                *window)
{
	GeditDocument *active_document;

	active_document = gedit_window_get_active_document (window);
	if (active_document != NULL)
	{
		gedit_document_set_language (active_document, language);
	}
}

void
_gedit_cmd_view_highlight_mode (GSimpleAction *action,
                                GVariant      *parameter,
                                gpointer       user_data)
{
	GeditWindow *window = GEDIT_WINDOW (user_data);
	GeditHighlightModeDialog *dialog;
	GeditHighlightModeSelector *selector;
	GeditDocument *active_document;

	dialog = GEDIT_HIGHLIGHT_MODE_DIALOG (gedit_highlight_mode_dialog_new (GTK_WINDOW (window)));
	selector = gedit_highlight_mode_dialog_get_selector (dialog);

	active_document = gedit_window_get_active_document (window);
	if (active_document != NULL)
	{
		GtkSourceLanguage *language;

		language = gedit_document_get_language (active_document);
		gedit_highlight_mode_selector_select_language (selector, language);
	}

	g_signal_connect_object (selector,
				 "language-selected",
				 G_CALLBACK (language_selected_cb),
				 window,
				 0);

	gtk_widget_show (GTK_WIDGET (dialog));
}

/* ex:set ts=8 noet: */
