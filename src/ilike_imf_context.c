/*
 * vim:ts=8:sw=3:sts=8:noexpandtab:cino=>5n-3f0^-2{2
 */

/*
 * Copyright (C) 2007-2009 Instituto Nokia de Tecnologia (INdT)
 *
 * Contact: Eduardo Lima (Etrunko) <eduardo.lima@openbossa.org>
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
#include <Ecore_IMF.h>
#include <eina/eina_list.h>

#include <stdio.h>
#include <stdlib.h>

typedef struct _ILike_IMF_Context ILike_IMF_Context;

struct _ILike_IMF_Context {
   int internal_focus_set;
};

static void _ilike_imf_context_del(Ecore_IMF_Context *ctx);
static void _ilike_imf_context_show(Ecore_IMF_Context *ctx);
static void _ilike_imf_context_hide(Ecore_IMF_Context *ctx);
static void _ilike_imf_context_focus_in(Ecore_IMF_Context *ctx);
static void _ilike_imf_context_focus_out(Ecore_IMF_Context *ctx);
static void _ilike_imf_context_input_mode_set(Ecore_IMF_Context *ctx, Ecore_IMF_Input_Mode input_mode);
static int _ilike_imf_context_filter_event(Ecore_IMF_Context *ctx, Ecore_IMF_Event_Type type, Ecore_IMF_Event *event);

#define SHOW_TIMEOUT    0.5

static Eina_List *ilike_imf_contexts = NULL;
static Ecore_Timer *show_timer = NULL;

#if 0
static void
_ilike_imf_on_vkb_commit(void *data, const char *text)
{
   Ecore_IMF_Context *ctx = data;
   //ILike_IMF_Context *ctxd = ecore_imf_context_data_get(ctx);

   ecore_imf_context_commit_event_add(ctx, text);
   // _ilike_imf_context_hide(ctx);
}
#endif

static ILike_IMF_Context *
_ilike_imf_context_new(void)
{
   ILike_IMF_Context *ctxd;

   ctxd = calloc(1, sizeof(ILike_IMF_Context));

   return ctxd;
}

static void
_ilike_imf_context_del(Ecore_IMF_Context *ctx)
{
   //ILike_IMF_Context *ctxd = ecore_imf_context_data_get(ctx);
   //_ilike_imf_context_client_canvas_set(ctx, NULL);
   ilike_imf_contexts = eina_list_remove(ilike_imf_contexts, ctx);
}

static Eina_Bool _show_cb(void *data)
{
    system("killall -USR1 mokowm");

    show_timer = NULL;
    return EINA_FALSE;
}

static void
_ilike_imf_context_show(Ecore_IMF_Context *ctx)
{
    /*
   ILike_IMF_Context *ctxd = ecore_imf_context_data_get(ctx);
   int has_surrounding;
   char *surrounding;
   int cpos;

   if (!ctxd->evas) return;

   has_surrounding = ecore_imf_context_surrounding_get(ctx,
						       &surrounding, &cpos);
   if (has_surrounding)
     ilike_vkbd_object_surrounding_set(ctxd->vkbd, surrounding, cpos);
   else
     ilike_vkbd_object_surrounding_set(ctxd->vkbd, NULL, 0);

   evas_object_show(ctxd->vkbd);
   */
    if (!show_timer)
        show_timer = ecore_timer_add(SHOW_TIMEOUT, _show_cb, NULL);
}

static void
_ilike_imf_context_hide(Ecore_IMF_Context *ctx)
{
    /*
   ILike_IMF_Context *ctxd = ecore_imf_context_data_get(ctx);
   if (!ctxd->evas) return;
   evas_object_hide(ctxd->vkbd);
   */
    if (show_timer) {
        ecore_timer_del(show_timer);
        show_timer = NULL;
    }

    system("killall -USR2 mokowm");
}

static void
_ilike_imf_context_focus_in(Ecore_IMF_Context *ctx)
{
   ILike_IMF_Context *ctxd = ecore_imf_context_data_get(ctx);
   _ilike_imf_context_show(ctx);
   ctxd->internal_focus_set = 1;
   //evas_object_focus_set(ctxd->vkbd, 1);
}

static void
_ilike_imf_context_focus_out(Ecore_IMF_Context *ctx)
{
   ILike_IMF_Context *ctxd = ecore_imf_context_data_get(ctx);
   if (ctxd->internal_focus_set)
     ctxd->internal_focus_set = 0;
   else
     {
	_ilike_imf_context_hide(ctx);
	//evas_object_focus_set(ctxd->vkbd, 0);
     }
}

static void
_ilike_imf_context_input_mode_set(Ecore_IMF_Context *ctx, Ecore_IMF_Input_Mode input_mode)
{
   //ILike_IMF_Context *ctxd = ecore_imf_context_data_get(ctx);
   /*
   ilike_vkbd_object_password_mode_set(ctxd->vkbd,
				       (input_mode & ECORE_IMF_INPUT_MODE_INVISIBLE) ? 1 : 0);
    */            
}

static int
_ilike_imf_context_filter_event(Ecore_IMF_Context *ctx, Ecore_IMF_Event_Type type, Ecore_IMF_Event *event)
{
   if (type == ECORE_IMF_EVENT_MOUSE_UP)
      _ilike_imf_context_show(ctx);

   return 0;
}

static const
Ecore_IMF_Context_Info ilike_imf_info =
{
   "ilike-input-method", /* ID */
   "ILike Input Method", /* Description */
   "*",                  /* Default locales */
   NULL,                 /* Canvas type */
   0                     /* Canvas required */
};

static Ecore_IMF_Context_Class ilike_imf_class =
{
   NULL,                                   /* add */
   _ilike_imf_context_del,                 /* del */
   NULL,                                   /* client_window_set */
   NULL,                                   /* client_canvas_set */
   _ilike_imf_context_show,                /* show */
   _ilike_imf_context_hide,                /* hide */
   NULL,                                   /* get_preedit_string */
   _ilike_imf_context_focus_in,            /* focus_in */
   _ilike_imf_context_focus_out,           /* focus_out */
   NULL,                                   /* reset */
   NULL,                                   /* cursor_position_set */
   NULL,                                   /* use_preedit_set */
   _ilike_imf_context_input_mode_set,      /* input_mode_set */
   _ilike_imf_context_filter_event         /* filter_event */
};

Ecore_IMF_Context* imf_module_exit(void)
{
    return NULL;
}

Ecore_IMF_Context *imf_module_create(void)
{
   Ecore_IMF_Context *ctx;
   ILike_IMF_Context *ctxd;

   ctxd = _ilike_imf_context_new();
   if (!ctxd) return NULL;
   ctx = ecore_imf_context_new(&ilike_imf_class);
   ecore_imf_context_data_set(ctx, ctxd);
   ilike_imf_contexts = eina_list_append(ilike_imf_contexts, ctxd);
   return ctx;
}

EAPI Eina_Bool imf_module_init(void)
{
   ecore_imf_module_register(&ilike_imf_info, imf_module_create, imf_module_exit);
   return EINA_TRUE;
}

EINA_MODULE_INIT(imf_module_init);
