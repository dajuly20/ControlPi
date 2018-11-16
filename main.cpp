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
//#include <boost/regex.hpp>
#include "pifacedigital.h"
#include "boolLogicParser.h"
#include "regReplaceExtension.h"
static volatile int keepRunning = 1;

using namespace std;

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


uint8_t invertRead(int i){
   return ~pifacedigital_digital_read(i)&1;
}
            
std::string regexCallback(const std::smatch& m) {
    cout << "call on: " << m[1].str() << endl;
    string deoendantStr = m[1].str();
    
    bool dependantState;
    
    char frstChar = deoendantStr.at(0);
    char scndChar = deoendantStr.at(1);
    char thrdChar = deoendantStr.at(2);
    char frthChar = deoendantStr.at(3);
   
    // Get number of Var.
    int dependantNum = frthChar - '0';
    
    
    // Input or Outupt
    if(frstChar == 'I'){  
       dependantState = invertRead(dependantNum);
       cout << "its input!" << dependantNum << " its state is: " << dependantState << endl;    
        
    }
    else if(frstChar == 'O'){
        cout << "its output" << dependantNum;    
        cout << "Don't know how to determine output state for now..";
        
    }else{
        cout << "FEHLER. This should never happen.. ";
    }
    
        return std::to_string(dependantState);
    // Todo second and third chars are ignored for now.
    //      and have to be implemented.
    
    
    
    
    
    cout << endl;
    
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
    
     //string format like:  a0 = "!0 & 1;";
            
        typedef std::string::const_iterator It;
        It f(input.begin()), l(input.end());
        parser<It> p;

        try
        {
            expr result;
            bool ok = qi::phrase_parse(f,l,p > ';',qi::space,result);

            if (!ok)
                std::cerr << "invalid inputjw\n";
            else
            {
                
                std::cout << "JWresult:\t" << result << "\n";
                std::cout << "JWevaluated:\t" << evaluate(result) << "\n";
                return evaluate(result);
            
            }

        } catch (const qi::expectation_failure<It>& e)
        {
            std::cerr << "expectation_failure at '" << std::string(e.first, e.last) << "'\n";
        }  
}


uint8_t parseInputs(){
      // Cut the string in halves at the "=" sign
    std::string delimiter = "=";

    size_t found;
    uint8_t outputByte = 0;
    
    //Strings shall be read as a array of strings 
    //string rawFullInput="Ora0=!(![Ira0] & [Ira1]) | [Ira2] & [Ira3];";  
     const std::string inputs[] = { 
        std::string("Ora0=!(![Ira0] & [Ira1]) | [Ira2] & [Ira3];"),
        std::string("Ora1=[Ira1];"),
        std::string("Ora2=[Ira2];"),
        std::string("Ora3=![Ira3];"),
        std::string("") // marker
    };

     
    for (std::string rawFullInput: inputs) { 

        // Splits the Locig into the Output to use (e.g. Ora0) and the rawLogicString
        if ((found = rawFullInput.find("=")) != string::npos){
           string hwOutputStr        = rawFullInput.substr(0,found);
           int hwOutputNumber = hwOutputStr.at(3) - '0';

           string rawLogicString  = rawFullInput.substr(found+1, string::npos);

           cout << "HardwareOutput is: " << hwOutputNumber << endl;
           cout << "RawLogicString is: " << rawLogicString << endl;

           string outLogicString  = regex_replace(rawLogicString, regex("\\[([I][rv][a-z][0-8])\\]"),
                                    regexCallback);
           cout << "Resulting logic string is: " << outLogicString << endl;

           bool parsedOut = parseLogic(outLogicString);

           // Adds up the one Bits
           // e.g. Output 3 is the third bit from right, so 2^3 * 1 or 0
           outputByte += int(parsedOut) * pow(2, hwOutputNumber); 
           cout << "Output Byte is now " << int(outputByte) << endl;
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

    signal(SIGINT, intHandler);
    
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
    cout << endl << endl << "ControlPi started!" << endl; 
    printf("Opening piface digital connection at location %d\n", hw_addr);
    pifacedigital_open(hw_addr);



    /**
     * Enable interrupt processing (only required for all blocking/interrupt methods).
     * Reverse the return value of pifacedigital_enable_interrupts() to be consistent
     * with the variable name "interrupts_enabled". (the function returns 0 on success)
     */
    if (interrupts_enabled = !pifacedigital_enable_interrupts())
        printf("Interrupts enabled.\n");
    else
        printf("Could not enable interrupts. Try running using sudo to enable PiFaceDigital interrupts.\n");


    /**
     * Bulk set all 8 outputs at once using a hexidecimal
     * representation of the inputs as an 8-bit binary
     * number, where each bit represents an output from
     * 0-7
     */
    /* Set all outputs off (00000000) */
    printf("Setting all outputs off\n");
    //pifacedigital_write_reg(0x00, OUTPUT, hw_addr);
    //sleep(1);

    /* Set output states to alternating on/off (10101010) */
    printf("Setting outputs to 10101010\n");
    //pifacedigital_write_reg(0xaa, OUTPUT, hw_addr);
    //sleep(1);

    /* Set output states to alternating off/on (01010101) */
    //printf("Setting outputs to 01010101\n");
    //pifacedigital_write_reg(0x55, OUTPUT, hw_addr);
    //sleep(1);

    /* Set all outputs off (000000) */
    //printf("Setting all outputs off\n");
    //pifacedigital_write_reg(0x00, OUTPUT, hw_addr);


    /**
     * Read/write single input bits
     */
    //uint8_t bit = pifacedigital_read_bit(0, OUTPUT, hw_addr);
    //printf("Reading bit 0: %d\n", bit);
    //sleep(1);
    //printf("Writing bit 0 to 0\n", bit);
    //pifacedigital_write_bit(0, 0, OUTPUT, hw_addr);


    /**
     * Set input pullups (must #include "mcp23s17.h")
     */
    /* pifacedigital_write_reg(0xff, GPPUB, hw_addr); */


    /**
     * Bulk read all inputs at once
     */
    inputs = pifacedigital_read_reg(INPUT, hw_addr);
    printf("Inputs: 0x%x\n", inputs);


    /**
     * Write each output pin individually
     */
    /*
    for (i = 0; i < 8; i++) {
        const char *desc;
        if (i <= 1) desc = "pin with attached relay";
        else desc = "pin";

        // Turn output pin i high 
        printf("Setting output %s %d HIGH\n", desc, (int)i);
        pifacedigital_digital_write(i, 1);
        sleep(1);

        //Turn output pin i low 
        printf("Setting output %s %d LOW\n", desc, (int)i);
        pifacedigital_digital_write(i, 0);
        sleep(1);
    }
*/

   

            //char Ora0[] = "Ira0*Ira1*Ira2*Ira2";
            
            //char str[] ="- This, a sample string.";
            
            
            
          
          
            
           
          
            
           
            
          
          
         // Should become => 
         // outChannel="ra0";  inputVars = "!(!1 & 0) || 1 & 0"
         // Res = parseLogic(inputVars) 
         // Dynamically select Ora0 as output and write res to it. 
         
          // test Logic parser.
          bool res = parseLogic("!0 & 0;");
          if(res){
              cout << "Result 1" << endl;
          }
          else{
              cout << "Result false" << endl;
          }
                 
    /**
     * Wait for input change interrupt.
     * pifacedigital_wait_for_input returns a value <= 0 on failure.
     */
    while(keepRunning ){
    if (interrupts_enabled) {
        printf("Waiting for input (press any button on the PiFaceDigital) CTRL + C to Abort\n");
        if (pifacedigital_wait_for_input(&inputs, -1, hw_addr) > 0){
        
            
            printf("Inputs: 0x%x\n", inputs);
             /**
            * Read each input pin individually
            * A return value of 0 is pressed.
            */
            uint8_t parsedOutputs = parseInputs();
            
            pifacedigital_write_reg(parsedOutputs, OUTPUT, hw_addr);
            
//            if ( invertRead(0) && invertRead(1) && invertRead(2) && invertRead(3)){
//               pifacedigital_digital_write(0, 1); 
//            }
//            else{
//                pifacedigital_digital_write(0, 0);
//            }
            
            for (i = 0; i < 8; i++) {
                uint8_t pinStateRev =  ~pifacedigital_digital_read(i) & 1;
                
                printf("Input %d value: %d\n", (int)i, (int)pinStateRev);
            }
        }else{
            printf("Can't wait for input. Something went wrong!\n");
        }
    }
    else{
        printf("Interrupts disabled, skipping interrupt tests.\n");
        keepRunning = 0;
    }

    }
    /**
     * Close the connection to the PiFace Digital
     */
    printf("CTRL + C Detected.. ");
    pifacedigital_close(hw_addr);
}
