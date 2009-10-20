/**
 * @addtogroup OVAL
 * @{
 * @addtogroup Probes
 * @{
 *
 * @file
 * Interface to most of the operations that need to be done in a probe;
 * processing input object, manipulating entities, creating output items
 *
 * @author "Daniel Kopecek" <dkopecek@redhat.com>
 */

/*
 * Copyright 2008 Red Hat Inc., Durham, North Carolina.
 * All Rights Reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software 
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Authors:
 *      Daniel Kopecek <dkopecek@redhat.com>
 */

#pragma once
#ifndef PROBE_API_H
#define PROBE_API_H

#include <seap.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <probe-entcmp.h>
#include "api/oval_definitions.h"
#include "api/oval_system_characteristics.h"
#include "api/oval_results.h"

/*
 * items
 */

/**
 * Build a new item according to the scpecified format.
 * @param fmt the desired format
 */
SEXP_t *probe_item_build (const char *fmt, ...);

/* SEXP_t *probe_item_creat (const char *name, SEXP_t *attrs, ...); */
/**
 * Create a new item consisting of a name, optional attributes argument
 * and an arbitrary number of entities.
 * @param name mandatory name argument
 * @param attrs optional attributes argument
 * @param ... arbitrary number of entity arguments
 */
#define probe_item_creat(name, attrs, ...) probe_obj_creat (name, attrs, __VA_ARGS__)

/**
 * Create a new item with just a name and optional attributes argument.
 * @param name item's name
 * @param attrs optional attributes argument
 */
SEXP_t *probe_item_new   (const char *name, SEXP_t *attrs);

/**
 * Add a new attribute to an item.
 * @param item the item to be modified
 * @param name name of the new attribute
 * @param val value of the new attribute
 */
SEXP_t *probe_item_attr_add (SEXP_t *item, const char *name, SEXP_t *val);

/**
 * Add a new entity to an item.
 * @param item the item to be modified
 * @param name name of the new entity
 * @param attrs optional attributes of the new entity
 * @param val value of the new entity
 */
SEXP_t *probe_item_ent_add (SEXP_t *item, const char *name, SEXP_t *attrs, SEXP_t *val);

/**
 * Set item's status.
 * @param obj the item to be modified
 * @param status the new status
 */
int probe_item_setstatus (SEXP_t *obj, oval_syschar_status_t status);

/**
 * Set status of an item's entity.
 * @param obj the item to be modified
 * @param name the name of the entity
 * @param n select the n-th occurence of an entity with the specified name
 * @param status the new status
 */
int probe_itement_setstatus (SEXP_t *obj, const char *name, uint32_t n, oval_syschar_status_t status);

/**
 *
 */
struct id_desc_t;

/**
 * Get a new unique id (within a probe) for an item.
 * @param id_desc pointer to a structure holding the global id context
 * @return a new id
 */
SEXP_t *probe_item_newid(struct id_desc_t *id_desc);

/**
 * Reset the item id generator.
 * @param id_desc pointer to a structure holding the global id context
 */
void probe_item_resetid(struct id_desc_t *id_desc);

/*
 * attributes
 */

/**
 * Create a new list of attributes.
 * @param name the name of the attribute
 * @param val the value of the attribute
 * @param ... there can be an arbitrary number of name - value pairs
 */
SEXP_t *probe_attr_creat (const char *name, const SEXP_t *val, ...);

/*
 * objects
 */

/**
 * Build a new object according to the scpecified format.
 * @param fmt the desired format
 */
SEXP_t *probe_obj_build (const char *fmt, ...);

/**
 * Create a new object consisting of a name, optional attributes argument
 * and an arbitrary number of entities.
 * @param name mandatory name argument
 * @param attrs optional attributes argument
 * @param ... arbitrary number of entity arguments
 */
SEXP_t *probe_obj_creat (const char *name, SEXP_t *attrs, ...);

/**
 * Create a new object with just a name and optional attributes argument.
 * @param name object's name
 * @param attrs optional attributes argument
 */
SEXP_t *probe_obj_new   (const char *name, SEXP_t *attrs);

/**
 * Get an entity from an object.
 * @param obj the queried object
 * @param name the name of the entity
 * @param n select the n-th occurence of an entity with the specified name
 */
SEXP_t *probe_obj_getent (const SEXP_t *obj, const char *name, uint32_t n);

/**
 * Get the value of an object's entity.
 * The function respects the var_ref attribute and returns the currently selected value.
 * @param obj the queried object
 * @param name the name of the entity
 * @param n select the n-th occurence of an entity with the specified name
 */
SEXP_t *probe_obj_getentval  (const SEXP_t *obj, const char *name, uint32_t n);

/**
 * Get the list of values of an object's entity.
 * If the entity uses var_ref, there can be more than one value.
 * @param obj the queried object
 * @param name the name of the entity
 * @param n select the n-th occurence of an entity with the specified name
 * @param res the resulting value list is stored in this argument
 * @return number of values in the list stored in the res argument
 */
int     probe_obj_getentvals (const SEXP_t *obj, const char *name, uint32_t n, SEXP_t **res);

/**
 * Get the value of an object's attribute.
 * @param obj the queried object
 * @param name the name of the attribute
 */
SEXP_t *probe_obj_getattrval (const SEXP_t *obj, const char *name);

/**
 * Check whether the specified attribute exists.
 * @param obj the queried object
 * @param name the name of the attribute
 */
bool    probe_obj_attrexists (const SEXP_t *obj, const char *name);

/**
 * Set objects's status.
 * @param obj the object to be modified
 * @param status the new status
 */
int probe_obj_setstatus (SEXP_t *obj, oval_syschar_status_t status);

/**
 * Set status of an object's entity.
 * @param obj the object to be modified
 * @param name the name of the entity
 * @param n select the n-th occurence of an entity with the specified name
 * @param status the new status
 */
int probe_objent_setstatus (SEXP_t *obj, const char *name, uint32_t n, oval_syschar_status_t status);

/**
 * Get the name of an object.
 * @param obj the queried object
 */
char  *probe_obj_getname   (const SEXP_t *obj);

/**
 * Get the name of an object.
 * The name is stored in the provided buffer.
 * @param obj the queried object
 * @param buffer the buffer to store the name in
 * @param buflen the length of the buffer
 */
size_t probe_obj_getname_r (const SEXP_t *obj, char *buffer, size_t buflen);

/*
 * entities
 */

/**
 * Create a new list of entities.
 * @param name the name of the entity
 * @param attrs optional attributes
 * @param val the value of the entity
 * @param ... there can be an arbitrary number of name - attributes - value triples
 */
SEXP_t *probe_ent_creat (const char *name, SEXP_t *attrs, SEXP_t *val, ...);

/**
 * Create a new entity.
 * @param name the name of the entity
 * @param attrs optional attributes
 * @param val the value of the entity
 */
SEXP_t *probe_ent_creat1 (const char *name, SEXP_t *attrs, SEXP_t *val);

/**
 * Add a new attribute to an entity.
 * @param ent the entity to be modified
 * @param name name of the new attribute
 * @param val value of the new attribute
 */
SEXP_t *probe_ent_attr_add (SEXP_t *ent, const char *name, SEXP_t *val);

/**
 * Get the value of an entity.
 * The function respects the var_ref attribute and returns the currently selected value.
 * @param ent the queried entity
 */
SEXP_t *probe_ent_getval  (const SEXP_t *ent);

/**
 * Get the list of values of an entity.
 * If the entity uses var_ref, there can be more than one value.
 * @param ent the queried entity
 * @param res the resulting value list is stored in this argument
 * @return number of values in the list stored in the res argument
 */
int     probe_ent_getvals (const SEXP_t *ent, SEXP_t **res);

/**
 * Get the value of an entity's attribute.
 * @param ent the queried entity
 * @param name the name of the attribute
 */
SEXP_t *probe_ent_getattrval (const SEXP_t *ent, const char *name);

/**
 * Check whether the specified attribute exists.
 * @param ent the queried entity
 * @param name the name of the attribute
 */
bool    probe_ent_attrexists (const SEXP_t *ent, const char *name);

/**
 * Set the OVAL data type of an entity.
 * @param ent the queried entity
 * @param type the new data type
 */
int probe_ent_setdatatype (SEXP_t *ent, oval_datatype_t type);

/**
 * Get the OVAL data type of an entity.
 * @param ent the queried entity
 */
oval_datatype_t probe_ent_getdatatype (const SEXP_t *ent);

/**
 * Set entity's mask.
 * @param ent the queried entity
 * @mask the new mask
 */
int  probe_ent_setmask (SEXP_t *ent, bool mask);

/**
 * Get entity's mask.
 * @param ent the queried entity
 */
bool probe_ent_getmask (const SEXP_t *ent);

/**
 * Set entity's status.
 * @param ent the entity to be modified
 * @param status the new status
 */
int probe_ent_setstatus (SEXP_t *ent, oval_syschar_status_t status);

/**
 * Get entity status.
 * @param ent the queried entity
 */
oval_syschar_status_t probe_ent_getstatus (const SEXP_t *ent);

/**
 * Get the name of an entity.
 * @param ent the queried entity
 */
char  *probe_ent_getname   (const SEXP_t *ent);

/**
 * Get the name of an entity.
 * The name is stored in the provided buffer.
 * @param ent the queried entity
 * @param buffer the buffer to store the name in
 * @param buflen the length of the buffer
 */
size_t probe_ent_getname_r (const SEXP_t *ent, char *buffer, size_t buflen);

/**
 * Free the memory allocated by the probe_* functions.
 * @param obj the object to be freed
 */
void probe_free (SEXP_t *obj);

#define PROBE_EINVAL     1 /* Invalid type/value/format */
#define PROBE_ENOELM     2 /* Missing element */
#define PROBE_ENOVAL     3 /* Missing value */
#define PROBE_ENOATTR    4 /* Missing attribute */
#define PROBE_EINIT      5 /* Initialization failed */
#define PROBE_ENOMEM     6 /* No memory */
#define PROBE_EOPNOTSUPP 7 /* Not supported */
#define PROBE_ERANGE     8 /* Out of range */
#define PROBE_EDOM       9 /* Out of domain */
#define PROBE_EFAULT    10 /* Memory fault/NULL value */
#define PROBE_EACCES    11 /* Operation not perimitted */
#define PROBE_EFATAL   254 /* Unrecoverable error */
#define PROBE_EUNKNOWN 255 /* Unknown/Unexpected error */

/* FIXME */
#define OVAL_STATUS_ERROR        1
#define OVAL_STATUS_EXISTS       2
#define OVAL_STATUS_DOESNOTEXIST 3
#define OVAL_STATUS_NOTCOLLECTED 4

#define PROBECMD_STE_FETCH 1
#define PROBECMD_OBJ_EVAL  2

#include <probe-cache.h>

/**
 * @struct id_desc_t
 * Holds information for item ids generation.
 */
struct id_desc_t {
#ifndef HAVE_ATOMIC_FUNCTIONS
        pthread_mutex_t item_id_ctr_lock; ///< id counter lock
#endif
        int item_id_ctr; ///< id counter
};

/// Probe's global runtime information
typedef struct {
        /* Protocol stuff */
        SEAP_CTX_t *ctx; ///< protocol context for communication with the library
        int         sd; ///< file descriptor used for communication

        /* Object cache */
        pcache_t   *pcache; ///< probed objects cache
        pthread_rwlock_t pcache_lock; ///< cache lock

        struct id_desc_t id_desc; ///< id generation information

        /* probe main */
        void *probe_arg; ///< optional argument to probe_main()
} globals_t;

#if defined(HAVE_ATOMIC_FUNCTIONS)
#define GLOBALS_INITIALIZER { NULL, -1, NULL, PTHREAD_RWLOCK_INITIALIZER, {1}, NULL }
#else
#define GLOBALS_INITIALIZER { NULL, -1, NULL, PTHREAD_RWLOCK_INITIALIZER, {PTHREAD_MUTEX_INITIALIZER, 1}, NULL }
#endif

/// Probe's global runtime information
extern globals_t global;

#define READER_LOCK_CACHE pthread_rwlock_rdlock (&globals.pcache_lock)
#define WRITER_LOCK_CACHE pthread_rwlock_wrlock (&globals.pcache_lock)
#define READER_UNLOCK_CACHE pthread_rwlock_unlock (&globals.pcache_lock)
#define WRITER_UNLOCK_CACHE pthread_rwlock_unlock (&globals.pcache_lock)

#define SEAP_LOCK pthread_mutex_lock (&globals.seap_lock)
#define SEAP_UNLOCK pthread_mutex_unlock (&globals.seap_lock)

#endif /* PROBE_API_H */
