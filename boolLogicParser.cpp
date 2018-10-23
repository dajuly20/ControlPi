/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   boolLogicParser.cpp
 * Author: julian
 * 
 * Created on 15. Oktober 2018, 16:52
 */



// boolLoigicParser.cpp
//

#include "boolLogicParser.h"
#define LZZ_INLINE inline
eval::eval ()
           {}
bool eval::operator () (var const & v) const
    { 
        if (v == "1" || v=="T" || v=="t" || v=="true" || v=="True")
            return true;
        else if (v == "0" || v=="F" || v=="f" || v=="false" || v=="False")
            return false;
        return boost::lexical_cast<bool>(v); 
    }
bool eval::operator () (binop <op_and> const & b) const
    {
        return recurse(b.oper1) && recurse(b.oper2);
    }
bool eval::operator () (binop <op_or> const & b) const
    {
        return recurse(b.oper1) || recurse(b.oper2);
    }
bool eval::operator () (unop <op_not> const & u) const
    {
        return !recurse(u.oper1);
    }
printer::printer (std::ostream & os)
  : _os (os)
                                        {}
void printer::operator () (var const & v) const
                                        { _os << v; }
void printer::operator () (binop <op_and> const & b) const
                                                  { print(" & ", b.oper1, b.oper2); }
void printer::operator () (binop <op_or > const & b) const
                                                  { print(" | ", b.oper1, b.oper2); }
void printer::print (std::string const & op, expr const & l, expr const & r) const
    {
        _os << "(";
        boost::apply_visitor(*this, l);
        _os << op;
        boost::apply_visitor(*this, r);
        _os << ")";
    }
void printer::operator () (unop <op_not> const & u) const
    {
        _os << "(";
        _os << "!";
        boost::apply_visitor(*this, u.oper1);
        _os << ")";
    }
bool evaluate (expr const & e)
{ return boost::apply_visitor(eval(), e); }
std::ostream & operator << (std::ostream & os, expr const & e)
{ boost::apply_visitor(printer(os), e); return os; }
#undef LZZ_INLINE
