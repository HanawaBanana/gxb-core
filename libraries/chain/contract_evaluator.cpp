/*
    Copyright (C) 2018 gxb

    This file is part of gxb-core.

    gxb-core is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    gxb-core is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with gxb-core.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <graphene/chain/contract_evaluator.hpp>
#include <graphene/chain/account_object.hpp>
#include <graphene/chain/exceptions.hpp>
#include <graphene/chain/is_authorized_asset.hpp>
#include <graphene/chain/signature_object.hpp>

#include <graphene/chain/apply_context.hpp>
#include <graphene/chain/wasm_interface.hpp>
#include <graphene/chain/wast_to_wasm.hpp>


namespace graphene { namespace chain {

void_result contract_call_evaluator::do_evaluate(const contract_call_operation &op)
{ try {
    dlog("contract_call_evaluator do_evaluator");
    return void_result();
} FC_CAPTURE_AND_RETHROW((op)) }

void_result contract_call_evaluator::do_apply(const contract_call_operation &op)
{ try {
    dlog("call contract, name ${n}, method ${m}, data ${d}", ("n", op.name)("m", op.method)("d", op.data));
    dlog("contract_call_evaluator do_apply");

    const char *wast_code = op.data.c_str();
    std::vector<uint8_t> wasm = graphene::chain::wast_to_wasm(wast_code);
    auto code_id = fc::sha256::hash(wast_code, (uint32_t) strlen(wast_code));
    auto wasm_bytes = bytes(wasm.begin(), wasm.end());
    dlog("wast code ${c}, code_id ${i}", ("c", wast_code)("i", code_id));

    action a{1, 1, {}};
    apply_context ap{a};
    wasm_interface(graphene::chain::wasm_interface::vm_type::binaryen).apply(code_id, wasm_bytes, ap);
    dlog("wasm exec success");

    return void_result();
} FC_CAPTURE_AND_RETHROW((op)) }

} } // graphene::chain