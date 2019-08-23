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

/**
 * \file ast/stmt.h
 * \brief openQASM statements
 */
#pragma once

#include "base.h"
#include <functional>

namespace synthewareQ {
namespace ast {

  /**
   * \class synthewareQ::ast::Stmt
   * \brief Base class for openQASM statements
   */
  class Stmt : public ASTNode {
  public:
    Stmt(parser::Position pos) : ASTNode(pos) {}
    virtual ~Stmt() = default;
    virtual Stmt* clone() const = 0;
  };

  /**
   * \class synthewareQ::ast::MeasureStmt
   * \brief Class for measurement statements
   * \see synthewareQ::ast::Stmt
   */
  class MeasureStmt final : public Stmt {
    ptr<VarExpr> q_arg_; ///< the quantum bit|register
    ptr<VarExpr> c_arg_; ///< the classical bit|register

  public:
    MeasureStmt(parser::Position pos, ptr<VarExpr> q_arg, ptr<VarExpr> c_arg)
      : Stmt(pos)
      , q_arg_(std::move(q_arg))
      , c_arg_(std::move(c_arg))
    {}

    VarExpr& q_arg() { return *q_arg_; }
    VarExpr& c_arg() { return *c_arg_; }
    void set_q_arg(ptr<VarExpr> q_arg) { q_arg_ = std::move(q_arg); }
    void set_c_arg(ptr<VarExpr> c_arg) { c_arg_ = std::move(c_arg); }

    void accept(Visitor& visitor) override { visitor.visit(this); }
    std::ostream& pretty_print(std::ostream& os) const override {
      os << "measure " << *q_arg_ << " -> " << *c_arg_ << ";\n";
      return os;
    }
    MeasureStmt* clone() const override {
      return new MeasureStmt(pos_, ptr<VarExpr>(q_arg_->clone()), ptr<VarExpr>(c_arg_->clone()));
    }
  };

  /**
   * \class synthewareQ::ast::ResetStmt
   * \brief Class for reset statements
   * \see synthewareQ::ast::Stmt
   */
  class ResetStmt final : public Stmt {
    ptr<VarExpr> arg_; ///< the qbit|qreg

  public:
    ResetStmt(parser::Position pos, ptr<VarExpr> arg) : Stmt(pos), arg_(std::move(arg)) {}

    VarExpr& arg() { return *arg_; }
    void set_arg(ptr<VarExpr> arg) { arg_ = std::move(arg); }

    void accept(Visitor& visitor) override { visitor.visit(this); }
    std::ostream& pretty_print(std::ostream& os) const override {
      os << "reset " << *arg_ << ";\n";
      return os;
    }
    ResetStmt* clone() const override {
      return new ResetStmt(pos_, ptr<VarExpr>(arg_->clone()));
    }
  };

  /**
   * \class synthewareQ::ast::IfStmt
   * \brief Class for if statements
   * \see synthewareQ::ast::Stmt
   */
  class IfStmt final : public Stmt {
    symbol var_;     ///< classical register name
    int cond_;       ///< value to check against
    ptr<Stmt> then_; ///< statement to be executed if true

  public:
    IfStmt(parser::Position pos, symbol var, int cond, ptr<Stmt> then)
      : Stmt(pos)
      , var_(var)
      , cond_(cond)
      , then_(std::move(then))
    {}

    const symbol& var() const { return var_; }
    int cond() const { return cond_; }
    Stmt& then() { return *then_; }

    void accept(Visitor& visitor) override { visitor.visit(this); }
    std::ostream& pretty_print(std::ostream& os) const override {
      os << "if (" << var_ << "==" << cond_ << ") " << *then_;
      return os;
    }
    IfStmt* clone() const override {
      return new IfStmt(pos_, var_, cond_, ptr<Stmt>(then_->clone()));
    }
  };

  /**
   * \class synthewareQ::ast::Gate
   * \brief Statement sub-class for gate
   */
  class Gate : public Stmt {
  public:
    Gate(parser::Position pos) : Stmt(pos) {}
    virtual ~Gate() = default;
    virtual Gate* clone() const = 0;
  };

  /**
   * \class synthewareQ::ast::UGate
   * \brief Class for U gates
   * \see synthewareQ::ast::Gate
   */
  class UGate final : public Gate {
    ptr<Expr> theta_;  ///< theta angle
    ptr<Expr> phi_;    ///< phi angle
    ptr<Expr> lambda_; ///< lambda angle

    ptr<VarExpr> arg_; ///< quantum bit|register

  public:
    UGate(parser::Position pos, ptr<Expr> theta, ptr<Expr> phi, ptr<Expr> lambda, ptr<VarExpr> arg)
      : Gate(pos)
      , theta_(std::move(theta))
      , phi_(std::move(phi))
      , lambda_(std::move(lambda))
      , arg_(std::move(arg))
    {}

    Expr& theta() { return *theta_; }
    Expr& phi() { return *phi_; }
    Expr& lambda() { return *lambda_; }
    VarExpr& arg() { return *arg_; }

    void accept(Visitor& visitor) override { visitor.visit(this); }
    std::ostream& pretty_print(std::ostream& os) const override {
      os << "U(" << *theta_ << "," << *phi_ << "," << *lambda_ << ") " << *arg_ << ";";
      return os;
    }
    UGate* clone() const override {
      return new UGate(pos_,
                       ptr<Expr>(theta_->clone()),
                       ptr<Expr>(phi_->clone()),
                       ptr<Expr>(lambda_->clone()),
                       ptr<VarExpr>(arg_->clone()));
    }
  };

  /**
   * \class synthewareQ::ast::CNOTGate
   * \brief Class for CX gates
   * \see synthewareQ::ast::Gate
   */
  class CNOTGate final : public Gate {
    ptr<VarExpr> ctrl_; ///< control qubit|qreg
    ptr<VarExpr> tgt_;  ///< target qubit|qreg

  public:
    CNOTGate(parser::Position pos, ptr<VarExpr> ctrl, ptr<VarExpr> tgt)
      : Gate(pos)
      , ctrl_(std::move(ctrl))
      , tgt_(std::move(tgt))
    {}

    VarExpr& ctrl() { return *ctrl_; }
    VarExpr& tgt() { return *tgt_; }

    void accept(Visitor& visitor) override { visitor.visit(this); }
    std::ostream& pretty_print(std::ostream& os) const override {
      os << "CX " << *ctrl_ << *tgt_ << ";";
      return os;
    }
    CNOTGate* clone() const override {
      return new CNOTGate(pos_, ptr<VarExpr>(ctrl_->clone()), ptr<VarExpr>(tgt_->clone()));
    }
  };

  /**
   * \class synthewareQ::ast::BarrierGate
   * \brief Class for barrier gates
   * \see synthewareQ::ast::Gate
   */
  class BarrierGate final : public Gate {
    std::vector<ptr<VarExpr> > args_; ///< list of quantum bits|registers

  public:
    BarrierGate(parser::Position pos, std::vector<ptr<VarExpr> >&& args)
      : Gate(pos)
      , args_(std::move(args))
      {}

    int num_args() const { return args_.size(); }
    VarExpr& arg(int i) { return *(args_[i]); }
    void foreach_arg(std::function<void(VarExpr&)> f) {
      for (auto it = args_.begin(); it != args_.end(); it++) f(**it);
    }

    void accept(Visitor& visitor) override { visitor.visit(this); }
    std::ostream& pretty_print(std::ostream& os) const override {
      os << "barrier ";
      for (auto it = args_.begin(); it != args_.end(); it++) {
        os << (it == args_.begin() ? "" : ",") << **it;
      }
      os << ";";
      return os;
    }
    BarrierGate* clone() const override {
      std::vector<ptr<VarExpr> > tmp;
      for (auto it = args_.begin(); it != args_.end(); it++) {
        tmp.emplace_back(ptr<VarExpr>((*it)->clone()));
      }

      return new BarrierGate(pos_, std::move(tmp));
    }
  };

  /**
   * \class synthewareQ::ast::DeclaredGate
   * \brief Class for declared gate applications
   * \see synthewareQ::ast::Gate
   */
  class DeclaredGate final : public Gate {
    symbol id_;                         ///< gate identifier
    std::vector<ptr<Expr> > c_args_;    ///< list of classical arguments
    std::vector<ptr<VarExpr> > q_args_; ///< list of quantum arguments

  public:
    DeclaredGate(parser::Position pos,
                 symbol id,
                 std::vector<ptr<Expr> >&& c_args,
                 std::vector<ptr<VarExpr> >&& q_args)
      : Gate(pos)
      , id_(id)
      , c_args_(std::move(c_args))
      , q_args_(std::move(q_args))
    {}

    int num_cargs() const { return c_args_.size(); }
    int num_qargs() const { return q_args_.size(); }
    Expr& carg(int i) { return *(c_args_[i]); }
    VarExpr& qarg(int i) { return *(q_args_[i]); }
    void foreach_carg(std::function<void(Expr&)> f) {
      for (auto it = c_args_.begin(); it != c_args_.end(); it++) f(**it);
    }
    void foreach_qarg(std::function<void(VarExpr&)> f) {
      for (auto it = q_args_.begin(); it != q_args_.end(); it++) f(**it);
    }

    void accept(Visitor& visitor) override { visitor.visit(this); }
    std::ostream& pretty_print(std::ostream& os) const override {
      os << id_;
      if (c_args_.size() > 0) {
        os << "(";
        for (auto it = c_args_.begin(); it != c_args_.end(); it++) {
          os << (it == c_args_.begin() ? "" : ",") << **it;
        }
        os << ")";
      }
      os << " ";
      for (auto it = q_args_.begin(); it != q_args_.end(); it++) {
        os << (it == q_args_.begin() ? "" : ",") << **it;
      }
      os << ";";
      return os;
    }
    DeclaredGate* clone() const override {
      std::vector<ptr<Expr> > c_tmp;
      for (auto it = c_args_.begin(); it != c_args_.end(); it++) {
        c_tmp.emplace_back(ptr<Expr>((*it)->clone()));
      }

      std::vector<ptr<VarExpr> > q_tmp;
      for (auto it = q_args_.begin(); it != q_args_.end(); it++) {
        q_tmp.emplace_back(ptr<VarExpr>((*it)->clone()));
      }

      return new DeclaredGate(pos_, id_, std::move(c_tmp), std::move(q_tmp));
    }
  };

}
}
