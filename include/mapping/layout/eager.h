/*
 * This file is part of synthewareQ.
 *
 * MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include "ast/traversal.h"
#include "mapping/device.h"

#include <map>
#include <vector>
#include <set>

namespace synthewareQ {
namespace mapping {

  /** \brief Allocates qubits on demand prioritizing coupling fidelity */
  class EagerLayout final : public ast::PostVisitor {
  public:
    EagerLayout(Device& device) : PostVisitor(), device_(device) {
      couplings_ = device_.couplings();
    }

    layout generate(ast::Program& prog) {
      layout_ = layout();
      allocated_ = std::vector<bool>(device_.qubits_, false);
      access_paths_.clear();

      prog.accept(*this);

      for (auto ap : access_paths_) {
        auto i = 0;
        bool cont = layout_.find(ap) == layout_.end();
        while (cont) {
          if (i >= device_.qubits_) {
            std::cerr << "Error: can't fit program onto device " << device_.name_ << "\n";
            return layout_;
          } else if (!allocated_[i]) {
              layout_[ap] = i;
              allocated_[i] = true;
              cont = false;
          }

          i++;
        }
      }

      return layout_;
    }

    // Ignore gate declarations
    void visit(ast::GateDecl&) override { }

    void visit(ast::RegisterDecl& decl) override {
      if (decl.is_quantum()) {
        for (int i = 0; i < decl.size(); i++)
          access_paths_.insert(ast::VarAccess(decl.pos(), decl.id(), i));
      }
    }

    // Try to assign a coupling to the cnot
    void visit(ast::CNOTGate& gate) override {
      auto ctrl = gate.ctrl();
      auto tgt = gate.tgt();

      size_t ctrl_bit;
      size_t tgt_bit;
      for (auto& [coupling, f] : couplings_) {
        if (auto it = layout_.find(ctrl); it != layout_.end()) {
          if (it->second != coupling.first) continue;
          else ctrl_bit = it->second;
        } else if (!allocated_[coupling.first]) {
          ctrl_bit = coupling.first;
        } else {
          continue;
        }

        if (auto it = layout_.find(tgt); it != layout_.end()) {
          if (it->second != coupling.second) continue;
          else tgt_bit = it->second;
        } else if (!allocated_[coupling.second]) {
          tgt_bit = coupling.second;
        } else {
          continue;
        }

        layout_[ctrl] = ctrl_bit;
        layout_[tgt] = tgt_bit;
        allocated_[ctrl_bit] = true;
        allocated_[tgt_bit] = true;
        couplings_.erase(std::make_pair(coupling, f));
        break;
      }

    }

  private:
    Device device_;
    layout layout_;
    std::vector<bool> allocated_;
    std::set<ast::VarAccess> access_paths_;
    std::set<std::pair<coupling, double> > couplings_;
  };

  layout compute_eager_layout(Device& device, ast::Program& prog) {
    EagerLayout gen(device);
    return gen.generate(prog);
  }

}
}
