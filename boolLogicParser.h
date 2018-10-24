// boolLogicParser.h
//

#ifndef LZZ_boolLogicParser_h
#define LZZ_boolLogicParser_h
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/variant/recursive_wrapper.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

namespace qi= boost::spirit::qi;
namespace phx= boost::phoenix;
#define LZZ_INLINE inline
struct op_or
{
};
struct op_and
{
};
struct op_not
{
};
typedef std::string var;
template <typename tag>
struct binop;
template <typename tag>
struct unop;
typedef boost::variant <var, 
        boost::recursive_wrapper<unop <op_not> >, 
        boost::recursive_wrapper<binop<op_and> >,
        boost::recursive_wrapper<binop<op_or> >
        > expr;
template <typename tag>
struct binop
{
  explicit binop (expr const & l, expr const & r);
  expr oper1;
  expr oper2;
};
template <typename tag>
struct unop
{
  explicit unop (expr const & o);
  expr oper1;
};
struct eval : boost::static_visitor <bool>
{
  eval ();
  bool operator () (var const & v) const;
  bool operator () (binop <op_and> const & b) const;
  bool operator () (binop <op_or> const & b) const;
  bool operator () (unop <op_not> const & u) const;
private:
  template <typename T>
  bool recurse (T const & v) const;
};
struct printer : boost::static_visitor <void>
{
  printer (std::ostream & os);
  std::ostream & _os;
  void operator () (var const & v) const;
  void operator () (binop <op_and> const & b) const;
  void operator () (binop <op_or > const & b) const;
  void print (std::string const & op, expr const & l, expr const & r) const;
  void operator () (unop <op_not> const & u) const;
};
bool evaluate (expr const & e);
std::ostream & operator << (std::ostream & os, expr const & e);
template <typename It, typename Skipper = qi::space_type>
struct parser : qi::grammar <It, expr(), Skipper>
{
  parser ();
private:
  qi::rule <It, var() , Skipper> var_;
  qi::rule <It, expr(), Skipper> not_;
  qi::rule <It, expr(), Skipper> and_;
  qi::rule <It, expr(), Skipper> or_;
  qi::rule <It, expr(), Skipper> simple;
  qi::rule <It, expr(), Skipper> expr_;
};
template <typename tag>
binop <tag>::binop (expr const & l, expr const & r)
  : oper1 (l), oper2 (r)
                                                                      {}
template <typename tag>
unop <tag>::unop (expr const & o)
  : oper1 (o)
                                            {}
template <typename T>
bool eval::recurse (T const & v) const
        { return boost::apply_visitor(*this, v); }
template <typename It, typename Skipper>
parser <It, Skipper>::parser ()
  : parser::base_type (expr_)
        {
            using namespace qi;

            expr_  = or_.alias();

            or_  = (and_ >> '|'  >> or_ ) [ _val = phx::construct<binop<op_or > >(_1, _2) ] | and_   [ _val = _1 ];
            and_ = (not_ >> '&' >> and_)  [ _val = phx::construct<binop<op_and> >(_1, _2) ] | not_   [ _val = _1 ];
            not_ = ('!' > simple       )  [ _val = phx::construct<unop <op_not> >(_1)     ] | simple [ _val = _1 ];

            simple = (('(' > expr_ > ')') | var_);
            var_ = qi::lexeme[ +(alpha|digit) ];

            BOOST_SPIRIT_DEBUG_NODE(expr_);
            BOOST_SPIRIT_DEBUG_NODE(or_);
            BOOST_SPIRIT_DEBUG_NODE(and_);
            BOOST_SPIRIT_DEBUG_NODE(not_);
            BOOST_SPIRIT_DEBUG_NODE(simple);
            BOOST_SPIRIT_DEBUG_NODE(var_);
        }
#undef LZZ_INLINE
#endif
