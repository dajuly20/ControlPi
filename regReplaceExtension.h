// regReplaceExtension.h
//

#ifndef LZZ_regReplaceExtension_h
#define LZZ_regReplaceExtension_h
#include <cstdlib>
#include <iostream>
#include <string>
#include <regex>
#define LZZ_INLINE inline

    template <typename BidirIt, typename Traits, typename CharT, typename UnaryFunction>
    std::basic_string <CharT> regex_replace (BidirIt first, BidirIt last, std::basic_regex <CharT,Traits> const & re, UnaryFunction f);

    template <typename Traits, typename CharT, typename UnaryFunction>
    std::string regex_replace (std::string const & s, std::basic_regex <CharT,Traits> const & re, UnaryFunction f);

    template <typename BidirIt, typename Traits, typename CharT, typename UnaryFunction>
    std::basic_string <CharT> regex_replace (BidirIt first, BidirIt last, std::basic_regex <CharT,Traits> const & re, UnaryFunction f)
    {
      std::basic_string<CharT> s;

      typename std::match_results<BidirIt>::difference_type
          positionOfLastMatch = 0;
      auto endOfLastMatch = first;

      auto callback = [&](const std::match_results<BidirIt>& match)
      {
          auto positionOfThisMatch = match.position(0);
          auto diff = positionOfThisMatch - positionOfLastMatch;

          auto startOfThisMatch = endOfLastMatch;
          std::advance(startOfThisMatch, diff);

          s.append(endOfLastMatch, startOfThisMatch);
          s.append(f(match));

          auto lengthOfMatch = match.length(0);

          positionOfLastMatch = positionOfThisMatch + lengthOfMatch;

          endOfLastMatch = startOfThisMatch;
          std::advance(endOfLastMatch, lengthOfMatch);
      };

      std::regex_iterator<BidirIt> begin(first, last, re), end;
      std::for_each(begin, end, callback);

      s.append(endOfLastMatch, last);

      return s;
  }

    template <typename Traits, typename CharT, typename UnaryFunction>
    std::string regex_replace (std::string const & s, std::basic_regex <CharT,Traits> const & re, UnaryFunction f)
  {
      return regex_replace(s.cbegin(), s.cend(), re, f);
  }

#undef LZZ_INLINE
#endif
