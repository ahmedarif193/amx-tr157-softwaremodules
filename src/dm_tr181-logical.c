/****************************************************************************
**
** SPDX-License-Identifier: BSD-2-Clause-Patent
**
** SPDX-FileCopyrightText: Copyright (c) 2021 SoftAtHome
**
** Redistribution and use in source and binary forms, with or
** without modification, are permitted provided that the following
** conditions are met:
**
** 1. Redistributions of source code must retain the above copyright
** notice, this list of conditions and the following disclaimer.
**
** 2. Redistributions in binary form must reproduce the above
** copyright notice, this list of conditions and the following
** disclaimer in the documentation and/or other materials provided
** with the distribution.
**
** Subject to the terms and conditions of this license, each
** copyright holder and contributor hereby grants to those receiving
** rights under this license a perpetual, worldwide, non-exclusive,
** no-charge, royalty-free, irrevocable (except for failure to
** satisfy the conditions of this license) patent license to make,
** have made, use, offer to sell, sell, import, and otherwise
** transfer this software, where such license applies only to those
** patent claims, already acquired or hereafter acquired, licensable
** by such copyright holder or contributor that are necessarily
** infringed by:
**
** (a) their Contribution(s) (the licensed copyrights of copyright
** holders and non-copyrightable additions of contributors, in
** source or binary form) alone; or
**
** (b) combination of their Contribution(s) with the work of
** authorship to which such Contribution(s) was added by such
** copyright holder or contributor, if, at the time the Contribution
** is added, such addition causes such combination to be necessarily
** infringed. The patent license shall not apply to any other
** combinations which include the Contribution.
**
** Except as expressly stated above, no rights or licenses from any
** copyright holder or contributor is granted under this license,
** whether expressly, by implication, estoppel or otherwise.
**
** DISCLAIMER
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
** CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
** INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
** MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR
** CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
** USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
** AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
** LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
** ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
** POSSIBILITY OF SUCH DAMAGE.
**
****************************************************************************/
#include <stdlib.h>

#include <debug/sahtrace.h>

#include <amxc/amxc.h>
#include <amxp/amxp.h>
#include <amxd/amxd_dm.h>
#include <amxd/amxd_object.h>
#include <amxd/amxd_object_event.h>
#include <amxd/amxd_action.h>
#include <amxd/amxd_transaction.h>

#include "tr181-logical_priv.h"

static void logical_interface_status(amxd_object_t* obj, bool enable) {
    amxd_trans_t transaction;
    const char* alias = NULL;
    amxd_dm_t* dm = logical_get_dm();
    amxd_trans_init(&transaction);
    amxd_trans_set_attr(&transaction, amxd_tattr_change_ro, true);

    alias = amxc_var_constcast(cstring_t, amxd_object_get_param_value(obj, "Alias"));
    amxd_trans_select_object(&transaction, obj);
    if(enable) {
        amxd_trans_set_value(cstring_t, &transaction, "Status", "Up");
        SAH_TRACE_WARNING("Logical Interface[%s] change Status(Up)", alias);
    } else {
        amxd_trans_set_value(cstring_t, &transaction, "Status", "Down");
        SAH_TRACE_WARNING("Logical Interface[%s] change Status(Down)", alias);
    }
    amxd_trans_apply(&transaction, dm);
    amxd_trans_clean(&transaction);
}

void _logical_interface_enabled(UNUSED const char* const sig_name,
                                const amxc_var_t* const data,
                                UNUSED void* const priv) {
    amxd_dm_t* dm = logical_get_dm();
    amxd_object_t* obj = amxd_dm_signal_get_object(dm, data);
    logical_interface_status(obj, true);
}

void _logical_interface_disabled(UNUSED const char* const sig_name,
                                 const amxc_var_t* const data,
                                 UNUSED void* const priv) {
    amxd_dm_t* dm = logical_get_dm();
    amxd_object_t* obj = amxd_dm_signal_get_object(dm, data);
    logical_interface_status(obj, false);
}

void _logical_interface_added(UNUSED const char* const sig_name,
                              const amxc_var_t* const data,
                              UNUSED void* const priv) {
    amxd_dm_t* dm = logical_get_dm();
    amxd_object_t* obj = amxd_dm_signal_get_object(dm, data);
    amxd_object_t* inst = amxd_object_get_instance(obj, NULL, GET_UINT32(data, "index"));
    logical_interface_status(inst, amxd_object_get_value(bool, inst, "Enable", NULL));
}
