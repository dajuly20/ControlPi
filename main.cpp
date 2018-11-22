/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: julian
 *
 * Created on 13. Oktober 2018, 21:49
 */

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <regex>
#include <bitset>
#include <stdexcept>
//#include <boost/regex.hpp>
#include "/usr/local/include/pifacedigitalcpp.h"
#include "boolLogicParser.h"
#include "regReplaceExtension.h"

static volatile int keepRunning = 1;

using namespace std;

PiFaceDigital pfd;


//string tr(const boost::smatch &m) {
    //return m[0].str()[0] == ' ' ? "_" : ""; }

void PrintMatches(std::string str){
    //Regular expression to match Inputs like "Ira0" which are in [ parenthese ] 
    // First char must be an capital I.
    // Second either r or v (real or virtual)
    // Third  smallercase a-z
    // Fourth will be one digit.
    //using namespace boost;    
    //boost::regex reg ("\\[([I][rv][a-z][0-8])\\]");
    
    // Used when your searching a string
    //std::smatch matches;
    
    // Show true and false in output
    // std::cout << std::boolalpha;
     
    
    //string output = boost::regex_replace(str, boost::regex("\\[([I][rv][a-z][0-8])\\]"), tr);
   // cout << "After replacing: " << output << endl;
    // Determines if there is a match and match 
    // results are returned in matches
    
          
}
 

//
//while(std::regex_search(str, matches, reg)){
//        std::cout << "Checked for Results : " << 
//                matches.ready() << "\n";
//        
//        std::cout << "Are there no matches : " << 
//                matches.empty() << "\n";
//        
//        std::cout << "Number of matches : " << 
//                matches.size() << "\n";
//        
//        // Get the first match
//        std::cout << matches.str(1) << "\n";
//        
//        // Eliminate the previous match and create
//        // a new string to search
//        str = matches.suffix().str();
//        
//        std::cout << "\n";
//    }



void PrintMatches2(std::string str){
    
    std::regex reg ("\\[([I][rv][a-z][0-8])\\]");
    
    // This holds the first match
    std::sregex_iterator currentMatch(str.begin(),
            str.end(), reg);
    
    // Used to determine if there are any more matches
    std::sregex_iterator lastMatch;
    
    // While the current match doesn't equal the last
    while(currentMatch != lastMatch){
        std::smatch match = *currentMatch;
        std::cout << match.str() << "\n";
        currentMatch++;
    }
    std::cout << std::endl;
    
}

class replaceIdentifier2{
    PiFaceDigital pfd;
    
public:
   
    replaceIdentifier2(PiFaceDigital _pfd){
        this->pfd = _pfd;
    }
   
    std::string operator()(const std::smatch& m) {
        bool dbg = false;
        if(dbg) cout << "call on: " << m[1].str() << endl;
        string deoendantStr = m[1].str();

        bool dependantState;

        // Splits String in characters.
        char frstChar = deoendantStr.at(0);
        char scndChar = deoendantStr.at(1);
        char thrdChar = deoendantStr.at(2);
        char frthChar = deoendantStr.at(3);

        // explicit cast 4th character to integer 
        // (regular expression checks for numberic)
        int dependantNum = frthChar - '0';

        // Input or Outupt
        if(frstChar == 'i'){  
           dependantState = pfd.digital_read_inv(dependantNum);
           if(dbg) cout << "its input " << dependantNum << " its state is: " << dependantState << endl;        
        }

        else if(frstChar == 'o'){
            //cout << "its output" << dependantNum;    
            throw std::invalid_argument("Outputs not yet supportet as identifiers");
           // cout << "Don't know how to determine output state for now..";   
        }

        else{
            // Todo throw error! 
            cout << "FEHLER. This should never happen.. ";
            throw std::invalid_argument("Identifier has invalid structure");
        }

        return std::to_string(dependantState);
        // Todo second and third chars are ignored for now.
        //      and have to be implemented.

        //cout << endl;

      return "1";
    }
    
};

// Used in regex callback to replace Identifiers by their value
std::string replaceIdentifier(const std::smatch& m) {
    bool dbg = false;
    if(dbg) cout << "call on: " << m[1].str() << endl;
    string deoendantStr = m[1].str();
    
    bool dependantState;
    
    // Splits String in characters.
    char frstChar = deoendantStr.at(0);
    char scndChar = deoendantStr.at(1);
    char thrdChar = deoendantStr.at(2);
    char frthChar = deoendantStr.at(3);
   
    // explicit cast 4th character to integer 
    // (regular expression checks for numberic)
    int dependantNum = frthChar - '0';
    
    // Input or Outupt
    if(frstChar == 'i'){  
       dependantState = pfd.digital_read_inv(dependantNum);
       if(dbg) cout << "its input " << dependantNum << " its state is: " << dependantState << endl;        
    }
    
    else if(frstChar == 'o'){
        //cout << "its output" << dependantNum;    
        throw std::invalid_argument("Outputs not yet supportet as identifiers");
       // cout << "Don't know how to determine output state for now..";   
    }
    
    else{
        // Todo throw error! 
        cout << "FEHLER. This should never happen.. ";
        throw std::invalid_argument("Identifier has invalid structure");
    }
    
    return std::to_string(dependantState);
    // Todo second and third chars are ignored for now.
    //      and have to be implemented.

    //cout << endl;
    
  return "1";
}


int testBoolLogic ()
{
    const std::string inputs[] = { 
        std::string("true & false;"),
        std::string("true & !false;"),
        std::string("!true & false;"),
        std::string("true | false;"),
        std::string("true | !false;"),
        std::string("!true | false;"),

        std::string("T&F;"),
        std::string("T&!F;"),
        std::string("!T&F;"),
        std::string("T|F;"),
        std::string("T|!F;"),
        std::string("!T|F;"),
        std::string("") // marker
    };

    for (const std::string *i = inputs; !i->empty(); ++i)
    {
        typedef std::string::const_iterator It;
        It f(i->begin()), l(i->end());
        parser<It> p;

        try
        {
            expr result;
            bool ok = qi::phrase_parse(f,l,p > ';',qi::space,result);

            if (!ok)
                std::cerr << "invalid input\n";
            else
            {
                std::cout << "result:\t" << result << "\n";
                std::cout << "evaluated:\t" << evaluate(result) << "\n";
            }

        } catch (const qi::expectation_failure<It>& e)
        {
            std::cerr << "expectation_failure at '" << std::string(e.first, e.last) << "'\n";
        }

        if (f!=l) std::cerr << "unparsed: '" << std::string(f,l) << "'\n";
    }

    return 0; 
}

void intHandler(int dummy) {
    keepRunning = 0;
}


bool parseLogic(string input){
    bool dbg = false;
     //string format like:  a0 = "!0 & 1;";
            
        typedef std::string::const_iterator It;
        It f(input.begin()), l(input.end());
        parser<It> p;

        try
        {
            expr result;
            bool ok = qi::phrase_parse(f,l,p > ';',qi::space,result);

            if (!ok)
                std::cerr << "Logic string invalid" << input << endl;
            else
            {
                if(dbg) std::cout << "parseLogic: Input " << input << " resulting into" << result << " evaluated to " << evaluate(result) << endl;
                return evaluate(result);
            
            }

        } catch (const qi::expectation_failure<It>& e)
        {
            std::cerr << "parseLogic: expectation_failure at '" << std::string(e.first, e.last) << "'\n";
        }  
}


uint8_t parseIdentifiers(PiFaceDigital pfd){
      // Cut the string in halves at the "=" sign
    bool dbg = false;
    std::string delimiter = "=";

    size_t found;
    uint8_t outputByte = 0;
    
    //Strings shall be read as a array of strings 
    //string rawFullInput="Ora0=!(![Ira0] & [Ira1]) | [Ira2] & [Ira3];";  
     const std::string outputStrings[] = { 
        std::string("Ora0=![ira1];"),
        std::string("Ora1=[ira1];"),
        std::string("Ora2=[ira2];"),
        std::string("Ora3=![ira3];"),
        std::string("") // marker
    };

     
    // One OutputString might be Ora0=![Ira1] & [Ira0] | [Ira2];
    //                           ^^^^   ^^^^
    //                             |     |  
    //                          Output  Input    
    for (std::string outStr: outputStrings) { 

        // Splits the output string by the '=' sign in two parts:
        // 1. the Output to use (e.g. Ora0) 2nd the rawLogicString including used inputs in brackets e.g. ![Ira1] & [Ira0] | [Ira2];
        if ((found = outStr.find("=")) != string::npos){ // TODO "" string to char?
           string useOutStr = outStr.substr(0,found);
           int    useOutNum = useOutStr.at(3) - '0'; 

           string rawLogicString  = outStr.substr(found+1, string::npos);

           if(dbg) cout << "HardwareOutput is: " << useOutNum << endl;
           if(dbg) cout << "RawLogicString is: " << rawLogicString << endl;

           // Using as Functor to put in piFaceDigital object.
           replaceIdentifier2 rpi(pfd);
           
           // Calls a function for each match on "[ira0]" a string of 4 characters in '[' brackets,
           // 1st char of which may be i/o (in/out), 2nd r/v (real/virtual), 3rd hardware idendifier (a-z), 4re input identifier (0-8)
           // eventually every occurance of brackets should be replaced either by a 0 or 1. 
           // For the example state of (Ira1 = 0, Ira0 =1, Ira2 = 1), the example logic string would look like !0 & 1 | 1;
           string outLogicString  = regex_replace(rawLogicString, regex("\\[([io][rv][a-z][0-8])\\]"),
                                    rpi);
           if(dbg) cout << "Resulting logic string is: " << outLogicString << endl;

           // Eventually the example logic string (e.g. !0 & 1 | 1;) will be parsed to 1
           bool parsedOut = parseLogic(outLogicString);

           // Adds up the one Bits
           // e.g. Output 3 is the third bit from right, so 2^3 * 1 or 0
           outputByte += int(parsedOut) * pow(2, useOutNum); 
           cout << "Output" << useOutNum << " is " << int(parsedOut) << endl;
           if(dbg) cout << "Output Byte is now " << int(outputByte) << endl;
        }
        else{
            // Todo: Error, = sign not in string.
        }
    }
     cout << "returning output byte: " << int(outputByte) << endl;
    return outputByte;
    
}


int main( int argc, char *argv[] )
{
    uint8_t i = 0;          /**< Loop iterator */
    uint8_t inputs;         /**< Input bits (pins 0-7) */
    int hw_addr = 0;        /**< PiFaceDigital hardware address  */
    int interrupts_enabled; /**< Whether or not interrupts are enabled  */
    int enable_interrupts = 1; /**< Whether or not interrupts should be enabled  */
    
    signal(SIGINT, intHandler);
    signal(SIGKILL, intHandler);
    signal(SIGHUP, intHandler);
    signal(SIGTERM, intHandler);
    
    
    //testBoolLogic();
    /**
     * Read command line value for which PiFace to connect to
     */
    if (argc > 1) {
        hw_addr = atoi(argv[1]);
    }

    
    /**
     * Open piface digital SPI connection(s)
     */
    printf("Opening piface digital connection at location %d\n", hw_addr);
    
  // Create Instance of pfd
    PiFaceDigital pfd(hw_addr, enable_interrupts);

    if(!pfd.init_success()){
        cout << "Error: Could not open PiFaceDigital" << endl;
        cout << "Is the device properly attached? " << endl; 
        return -1;
    }
    // Initially set all outputs to 0
    pfd.write_reg(0x00, OUTPUT, hw_addr);


    /**
     * Enable interrupt processing (only required for all blocking/interrupt methods).
     * Reverse the return value of pifacedigital_enable_interrupts() to be consistent
     * with the variable name "interrupts_enabled". (the function returns 0 on success)
     */
    if (pfd.interrupts_enabled()){
        printf("Interrupts enabled.\n");
    }else{
        printf("Could not enable interrupts. Are you in group spi and gpio? Try running using sudo to enable PiFaceDigital interrupts\n");
        return -1;
    }

  
    /**
     * Bulk read all inputs at once
     */
    inputs = pfd.read_reg(PiFaceDigital::OUT, hw_addr);
    printf("Inputs: 0x%x\n", inputs);

    inputs = pfd.read_reg(PiFaceDigital::OUT, hw_addr);
    printf("Outputs: 0x%x\n", inputs);


    /**
     * Wait for input change interrupt.
     * pifacedigital_wait_for_input returns a value <= 0 on failure.
     */
    while(keepRunning ){
    if (pfd.interrupts_enabled()) {
        printf("\n\nWaiting for input (press any button on the PiFaceDigital)\n");
        if (pfd.wait_for_input(&inputs, -1, hw_addr) > 0){
        
            
            //printf("Inputs: 0x%x\n", inputs);
             /**
            * Read each input pin individually
            * A return value of 0 is pressed.
            */
            uint8_t parsedOutputs = parseIdentifiers(pfd);
            
            pfd.write_reg(parsedOutputs, OUTPUT, hw_addr);
            inputs = pfd.read_reg(OUTPUT, hw_addr);
            printf("Outputs: 0x%x\n", inputs);
//            if ( invertRead(0) && invertRead(1) && invertRead(2) && invertRead(3)){
//               pfd.digital_write(0, 1); 
//            }
//            else{
//                pfd.digital_write(0, 0);
//            }
            
            for (i = 0; i < 8; i++) {
                uint8_t pinStateRev =  pfd.digital_read_inv(i); 
                
                printf("Input %d value: %d\n", (int)i, (int)pinStateRev);
            }
        }else{
            printf("Can't wait for input. Something went wrong!\n");
        }
    }
    else{
        printf("Interrupts disabled. Aborting.\n");
        keepRunning = 0;
    }

    }
    /**
     * Close the connection to the PiFace Digital 
     * pfd object goes out of scope, destructor does the rest!
     */
    
    pfd.write_reg(0x00, OUTPUT, hw_addr);
    
}
