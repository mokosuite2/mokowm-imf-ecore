/*
 * MokoWM input method keyboard activator
 * IM Context -- module entry point
 * 
 * Thanks to Eduardo Lima (Etrunko) <eduardo.lima@openbossa.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <Ecore.h>
#include <Ecore_X.h>
#include <Ecore_Evas.h>
#include <Evas.h>
#include <Ecore_IMF.h>
#include <eina/eina_list.h>

#include <stdio.h>
#include <stdlib.h>

typedef struct {
   Ecore_X_Window window;
} MokoWM_IMF_Context;

static void _mokowm_imf_context_del(Ecore_IMF_Context *ctx);
static void _mokowm_imf_context_show(Ecore_IMF_Context *ctx);
static void _mokowm_imf_context_hide(Ecore_IMF_Context *ctx);
static void _mokowm_imf_context_focus_in(Ecore_IMF_Context *ctx);
static void _mokowm_imf_context_window_set(Ecore_IMF_Context *ctx, void *window);
static int _mokowm_imf_context_filter_event(Ecore_IMF_Context *ctx, Ecore_IMF_Event_Type type, Ecore_IMF_Event *event);

#define SHOW_TIMEOUT    0.5

static Eina_List *mokowm_imf_contexts = NULL;
static Ecore_Timer *show_timer = NULL;

static MokoWM_IMF_Context* _mokowm_imf_context_new(void)
{
   MokoWM_IMF_Context *ctxd;

   ctxd = calloc(1, sizeof(MokoWM_IMF_Context));

   return ctxd;
}

static void _mokowm_imf_context_del(Ecore_IMF_Context *ctx)
{
   _mokowm_imf_context_window_set(ctx, NULL);
   mokowm_imf_contexts = eina_list_remove(mokowm_imf_contexts, ctx);
}

static Eina_Bool _show_cb(void *data)
{
    system("killall -USR1 mokowm");

    show_timer = NULL;
    return EINA_FALSE;
}

static void _mokowm_imf_context_window_set(Ecore_IMF_Context *ctx, void *window)
{
    MokoWM_IMF_Context *ctxd = ecore_imf_context_data_get(ctx);

    if (window) {
        Evas_Object* obj = window;
        Evas* evas = evas_object_evas_get(obj);
        Ecore_Evas* ee = ecore_evas_ecore_evas_get(evas);
        Ecore_X_Window xwin = ecore_evas_window_get(ee);

        ctxd->window = xwin;
    }

    else {
        ctxd->window = 0;
        _mokowm_imf_context_hide(ctx);
    }
}

static void _mokowm_imf_context_show(Ecore_IMF_Context *ctx)
{
    if (!show_timer)
        show_timer = ecore_timer_add(SHOW_TIMEOUT, _show_cb, NULL);
}

static void _mokowm_imf_context_hide(Ecore_IMF_Context *ctx)
{
    if (show_timer) {
        ecore_timer_del(show_timer);
        show_timer = NULL;
    }

    system("killall -USR2 mokowm");
}

static void _mokowm_imf_context_focus_in(Ecore_IMF_Context *ctx)
{
    _mokowm_imf_context_show(ctx);
}

static void _mokowm_imf_context_focus_out(Ecore_IMF_Context *ctx)
{
    MokoWM_IMF_Context *ctxd = ecore_imf_context_data_get(ctx);
    //EINA_LOG_ERR("FOCUS OUT: context = %p, window = 0x%x, focus = 0x%x",
    //    ctx, ctxd->window, ecore_x_window_focus_get());

    if (ctxd->window) {
        if (ctxd->window != ecore_x_window_focus_get())
            _mokowm_imf_context_hide(ctx);
    }
}

#if 0
static void _mokowm_imf_context_input_mode_set(Ecore_IMF_Context *ctx, Ecore_IMF_Input_Mode input_mode)
{
    // TODO
}
#endif

static int _mokowm_imf_context_filter_event(Ecore_IMF_Context *ctx, Ecore_IMF_Event_Type type, Ecore_IMF_Event *event)
{
    // TODO
    /*
    if (type == ECORE_IMF_EVENT_MOUSE_UP)
        _mokowm_imf_context_show(ctx);
    */

    return 0;
}

static const
Ecore_IMF_Context_Info mokowm_imf_info =
{
    "mokowm-input-method", /* ID */
    "MokoWM Input Method", /* Description */
    "*",                  /* Default locales */
    NULL,                 /* Canvas type */
    0                     /* Canvas required */
};

static Ecore_IMF_Context_Class mokowm_imf_class =
{
    NULL,                                   /* add */
    _mokowm_imf_context_del,                 /* del */
    _mokowm_imf_context_window_set,          /* client_window_set */
    NULL,                                   /* client_canvas_set */
    _mokowm_imf_context_show,                /* show */
    _mokowm_imf_context_hide,                /* hide */
    NULL,                                   /* get_preedit_string */
    _mokowm_imf_context_focus_in,            /* focus_in */
    _mokowm_imf_context_focus_out,                                  /* focus_out */
    NULL,                                   /* reset */
    NULL,                                   /* cursor_position_set */
    NULL,                                   /* use_preedit_set */
    NULL,                                  /* input_mode_set */
    _mokowm_imf_context_filter_event         /* filter_event */
};

Ecore_IMF_Context* imf_module_exit(void)
{
    return NULL;
}

Ecore_IMF_Context *imf_module_create(void)
{
    Ecore_IMF_Context *ctx;
    MokoWM_IMF_Context *ctxd;

    ctxd = _mokowm_imf_context_new();
    if (!ctxd) return NULL;
    ctx = ecore_imf_context_new(&mokowm_imf_class);
    ecore_imf_context_data_set(ctx, ctxd);
    mokowm_imf_contexts = eina_list_append(mokowm_imf_contexts, ctxd);
    return ctx;
}

EAPI Eina_Bool imf_module_init(void)
{
    ecore_imf_module_register(&mokowm_imf_info, imf_module_create, imf_module_exit);
    return EINA_TRUE;
}

EINA_MODULE_INIT(imf_module_init);
