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

#include <iostream> // cout
#include <string>   // std::string
#include <signal.h> // signal
#include <fstream>  // ifstream 

#include <regex>    // regex_replace
#include <stdexcept> // throw

#include "pifacedigitalcpp.h"
#include "boolLogicParser.h"    
#include "regReplaceExtension.h" // Extended reg-replace for inserting callback function.
#include "src/ChannelEntitys/Channel_Entity.h"
#include "src/ChannelEntitys/Channel_Entities_PiFace.h"
#include "src/IOChannels/IO_Channel.h"
#include "src/IOChannels/IO_Channel_Hw_PiFace.h"
#include "src/IOChannels/IO_Channel_Virtual_Memory.h"
#include "IO_Channel_AccesWrapper.h"
#include "src/IOChannels/IO_Channel_Virtual_Timer.h"

using namespace std;

// Globals 
static volatile int  keepRunning = 1;     //Used to interrupt mainloop
static volatile bool configRead  = false; //Used to re-read configuration

 
/*
 Functor to inject the IO Object into the replace identifier function.
 */
typedef shared_ptr<IO_Channel_AccesWrapper> IO_Channel_AccessWrapperPTR;

class replaceIdentifier{
    IO_Channel_AccesWrapper& chnl;
    
public:
   
//    replaceIdentifier(PiFaceDigital* _pfd){
//        this->pfd = _pfd;
//    }
    
    replaceIdentifier(IO_Channel_AccesWrapper& _chnl) : chnl(_chnl){
    }
//        this->chnl = &_chnl;
    
   
    
    // TODO Change to return char? 
    std::string operator()(const std::smatch& match) {
        bool dbg = true;
        if(dbg) cout << "call on: " << match[1].str() << endl;
        string dependantStr = match[1].str();

        bool dependantState;

        // Splits String in characters.
        char frstChar = dependantStr.at(0);
        char scndChar = dependantStr.at(1);
        char thrdChar = dependantStr.at(2);
        //char frthChar = dependantStr.at(3);

        // explicit cast 3th character to integer 
        // (regular expression checks for numberic)
        int dependantNum = thrdChar - '0';

        dependantState = chnl[frstChar][scndChar]->read_pin(dependantNum);
        
        if(dbg) cout << "FrstChar: " << frstChar << endl << "ScndChar: " << scndChar << endl << " Number:" << thrdChar << endl << endl;
         
        /*
         Removed 27.12.2018 ... 
        // Input or Outupt
        if(scndChar == 'i'){  
           dependantState = pfd->read_pin(dependantNum, PiFaceDigital::IN);
           if(dbg) cout << "its input " << dependantNum << " its state is: " << dependantState << endl;        
        }

        else if(scndChar == 'o'){
            //cout << "its output" << dependantNum;    
            dependantState = pfd->read_pin(dependantNum, PiFaceDigital::OUT);
            //throw std::invalid_argument("Outputs not yet supportet as identifiers");
           // cout << "Don't know how to determine output state for now..";   
        }

        else{
            // Todo throw error! 
            cout << "FEHLER. This should never happen.. ";
            throw std::invalid_argument("Identifier has invalid structure");
        }

         */
         
        return std::to_string(dependantState);
        // Todo second and third chars are ignored for now.
        //      and have to be implemented.

        //cout << endl;


    }
    
};

/*
 * Signal Handler
 * Sets the condition of the main loop to zero, 
 * to end the program.
 * 
 */
void intHandler(int dummy) {
    keepRunning = 0;
}

/* Signal Handler
 * Handles SIGUSR1 Signal to trigger re-read of config.
 */
void usrSigHandler(int dummy) {
    printf("\n Reveived reload signal.\n ");
    configRead = false;
}

/*
 * evaulateLogicString
 * Evaluates a given logic string e.g. !0 &1 | 0 ... ; to a boolean result. 
 */
bool evaluateLogicString(string input){
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

/*
 * parseIdentifiers
 * Pases a vector of strings, and replaces square brakets to their 
 * 
 * One row may come as follows:
 * One OutputString might be Ora0=![Ira1] & [Ira0] | [Ira2];
 *                           ^^^^   ^^^^
 *                             |     |  
 *                           Output  Input 
 * 
 * In above example the logic string for O-utput r-eal a(..z) 0(..9) will be parsed.
 * For that anything in square brackets will be replaced by a boolean value representing
 * the state of the related "pin" or the related signal.
 * 
 * So given a value of Ira1=0; Ira0=1 and Ira2 = 0 the above example would end up as follows: 
 * Ora0=!0 & 1 | 0 
 *
 */
void parseIdentifiers(IO_Channel_AccesWrapper& chnl, std::vector<std::string>& softLogic){
      // Cut the string in halves at the "=" sign
    bool dbg = true;
    std::string delimiter = "=";

    size_t found;
    //uint8_t outputByte = 0;

    
    cout << endl << endl << "Uising following config:" << endl << "---------------------------------------------" << endl;
    
    for (std::string outStr: softLogic) { 
        cout << outStr << endl;
    } 
    
    cout << endl <<  "---------------------------------------------" << endl << endl;
   
    for (std::string softLogicRow: softLogic) { 

        // Splits the output string by the '=' sign in two parts:
        // 1. the Output to use (e.g. Ora0) 2nd the rawLogicString including used inputs in brackets e.g. ![Ira1] & [Ira0] | [Ira2];
        if ((found = softLogicRow.find('=')) != string::npos){ // Changed "" to '' 27.12.2018. Remove comment when it works ;) 
            
            
            // The part before the '=' is the variable, to which whe outcome will be asigned to ==> 'asigned' part7
            // TODO Check for length of String
            string asignedEntityStr         = softLogicRow.substr(0,found);
            if(asignedEntityStr.size() != 3){
                throw std::invalid_argument("Error: Asignee needs to have 3 letters/digits in '"+asignedEntityStr+"'");
            }
            char   asigned_IOChannel        = asignedEntityStr.at(0);
            char   asigned_ChannelEntity    = asignedEntityStr.at(1);
            int    asigned_Pin              = asignedEntityStr.at(2) - '0'; 

           // The part after the '=' is the equation string. It consists of identifiers 
           //   (like [Ho0]) arithmetic operators ( &, | ) and round brakets. Also allowed  are literals (0 or 1)
           string equationString  = softLogicRow.substr(found+1, string::npos);

           if(dbg) cout << "HardwareOutput is: " << asigned_Pin << endl;
           if(dbg) cout << "RawLogicString is: " << equationString << endl;

           
           // Instantiate the replace-identifier Functor/Class that is used to
           //   inject the IO_Channel_AccessWrapper into the actual replace-function.
           replaceIdentifier rpi(chnl);
           
           // Calls a function for each match on "[ira0]" a string of 4 characters in '[' brackets,
           // 1st char of which may be i/o (in/out), 2nd r/v (real/virtual), 3rd hardware idendifier (a-z), 4re input identifier (0-8)
           // eventually every occurance of brackets should be replaced either by a 0 or 1. 
           // For the example state of (Ira1 = 0, Ira0 =1, Ira2 = 1), the example logic string would look like !0 & 1 | 1;
           string outLogicString  = regex_replace(equationString, regex("\\[([A-Z][a-z][0-8])\\]"),
                                    rpi);
           if(dbg) cout << "Resulting logic string is: " << outLogicString << endl;

           // Eventually the example logic string (e.g. !0 & 1 | 1;) will be parsed to 1
           bool parsedOut = evaluateLogicString(outLogicString);

           if(dbg) cout << "assigned IO_chnl: " << asigned_IOChannel << endl;
           if(dbg) cout << "asigned ChannelEntity:" << asigned_ChannelEntity << endl;
           if(dbg) cout << "assigned Pin: " << asigned_Pin << endl;
           if(dbg) cout << "parsed out is " << (parsedOut ? "true" : "false") << endl;
           
           chnl[asigned_IOChannel][asigned_ChannelEntity]->write_pin(parsedOut, asigned_Pin);
           
           
           // Adds up the one Bits
           // e.g. Output 3 is the third bit from right, so 2^3 * 1 or 0
           //outputByte += int(parsedOut) * pow(2, useOutNum); // TODO shift? 
           
//           if (int(parsedOut) == 1) {
//                outputByte |= 1 << asignedPin; // set
//            } else {
//                outputByte &= 0xff ^ (1 << asignedPin); // clear
//            }
          // cout << "Output" << asigned_Pin << " is " << int(parsedOut) << endl;
          // if(dbg) cout << "Output Byte is now " << int(outputByte) << endl;
        }
        else{
            // Todo: Error, = sign not in string.
        }
    }
    //cout << "returning output byte: " << int(outputByte) << endl;
    //return outputByte;
    
}

/*
 * stripWhite
 * Stripes any whitespace from given string.
 * (used for reading configuration)
 */     
std::string strip_white(const std::string& input)
{
   size_t b = input.find_first_not_of(' ');
   if (b == std::string::npos) b = 0;
   return input.substr(b, input.find_last_not_of(' ') + 1 - b);
}
 

/*
 * strip_comments
 * Strips anything starting from the first position of given delimiter.
 * e.g. "#" for Comments.
 */
std::string strip_comments(const std::string& input, const std::string& delimiters)
{
   return strip_white(input.substr(0, input.find_first_of(delimiters)));
}

/*
 * loadSoftLogic
 * Loads logic from given filename, and returns it as vector of strings.
 */
std::vector<std::string>  loadSoftLogic(std::string filename){
    
        std::ifstream data(filename);
        
        if(!data.is_open()){
           throw std::invalid_argument("Error: Couldnt load conf file: "+filename);
        }
	std::string line;
	std::vector<std::string> logic;

	while (std::getline(data, line))
	{
                std::string delimiters("#;");
                line = strip_comments(line, delimiters);
                if(line.size() > 0){
                    cout << "Adding Line: >>" << line << "<<" << endl;
                    logic.push_back(line+';');
                }
	}
        
        if(logic.size() == 0){
            throw std::invalid_argument("Error: Config file empty!");
        }
        
  
   std::vector<std::string> fallbackLogic = {
        std::string("Ora0=([ira0] | [ora0]) & ![ira1];"),
        std::string("Ora1=[ira1];"),
        std::string("Ora2=[ira2];"),
        std::string("Ora3=![ira3];")
    };
    
   
   return logic;
}
 

int main( int argc, char *argv[] )
{
    uint8_t i = 0;          /**< Loop iterator */
    uint8_t inputs;         /**< Input bits (pins 0-7) */
    uint8_t outputs;         /**< Input bits (pins 0-7) */
    
    // Register signalHandler

    signal(SIGINT,  intHandler);
    signal(SIGKILL, intHandler);
    signal(SIGHUP,  intHandler);
    signal(SIGTERM, intHandler);
    signal(SIGUSR1 ,usrSigHandler);
    

    
    /**
     * Read command line value for which PiFace to connect to
     */
//    if (argc > 1) {
//        hw_addr = atoi(argv[1]);
//    }

    
    /**
     * Open piface digital SPI connection(s)
     */
//    printf("Opening piface digital connection at location %d\n", hw_addr);
//    
        TimerCpp tu = TimerCpp();
 
        tu.setInterval([&]() {
                cout << "Hey.. After each 1s..." << endl;
            }, 1000); 
 
                    tu.setTimeout([&]() {
            cout << "Hey.. After 5.2s. But I will stop the timer!" << endl;
            tu.stop();
        }, 5200); 


    
    IO_Channel_AccesWrapper chnl;
    // Could Access now via (*myte.io_channels['H'])['i']->read_pin(0) 
    // But IO_Channel_AccessWrapper hides it away, and simplyfies access. so obj['H']['i']->member
    chnl.io_channels.insert(std::make_pair('H', new IO_Channel_Hw_PiFace()));
    chnl.io_channels.insert(std::make_pair('M', new IO_Channel_Virtual_Memory()));
    chnl.io_channels.insert(std::make_pair('T', new IO_Channel_Virtual_Timer()));
     
 
    int inputHi0 = (int) chnl['H']['i'][0]; // (*io_channels['H'])['i']->read_pin(0);
    cout << "Test Entity New: " <<  inputHi0 << endl;

    chnl['H']['o']->write_all(0xFF);
    sleep(1);
    
    // Initially set all outputs to 0
    chnl['H']['o']->write_all(0x00);



    /**
     * Enable interrupt processing (only required for all blocking/interrupt methods).
     * Reverse the return value of pifacedigital_enable_interrupts() to be consistent
     * with the variable name "interrupts_enabled". (the function returns 0 on success)
     */
//    if (pfd.interrupts_enabled()){
//        printf("Interrupts enabled.\n");
//    }else{
//        printf("Could not enable interrupts. Are you in group spi and gpio? Try running using sudo to enable PiFaceDigital interrupts\n");
//        return -1;
//    }

  
    /**
     * Bulk read all inputs at once
     */
    inputs = chnl['H']['i']->read_all();
    printf("Inputs: 0x%x\n", inputs);

    
    // Mandatory for interrupt funktion.
    inputs = chnl['H']['o']->read_all();
    printf("Outputs: 0x%x\n", inputs);

    
    
    
    std::string filename = "logic.conf";
    std::vector<std::string>  softLogic;
    
    /**
     * Wait for input change interrupt.
     * pifacedigital_wait_for_input returns a value <= 0 on failure.
     */
    // Initially read config and set Outputs accordingly.
    softLogic = loadSoftLogic(filename);
    configRead = true;
    //uint8_t parsedOutputs = 
    parseIdentifiers(chnl, softLogic);
    
    
    //chnl['H']['o']->write_all(parsedOutputs);
    
    
    
    // HIIIIIIIIIIIIIIIIIER WAR ICH ...
  //  chnl['H'].getIOChnl(). 
    chnl['H'].getIOChnl()->caching_enable();  
    
    while(keepRunning ){
    if (chnl['H'].getIOChnl()->interrupts_enabled()) {
        printf("\n\nWaiting for input (press any button on the PiFaceDigital)\n");
        if (chnl['H'].getIOChnl()->wait_for_interrupt()){
        
            if(configRead == false){
                softLogic = loadSoftLogic(filename);
                configRead = true;
                printf("\n\nConfig has been reloaded!\n");
            }
            
            /**
            * Read each input pin individually
            * A return value of 0 is pressed.
            */
            
            // Enable caching for pfd until parsing of identifiers is ready.
          
            chnl['H'].getIOChnl()->flush();
            //uint8_t parsedOutputs = 
            parseIdentifiers(chnl, softLogic);
         
            //chnl['H']['o']->write_all(parsedOutputs);
            outputs = chnl['H']['i']->read_all();
            printf("Outputs: 0x%x\n", outputs);
            
            chnl['H'].getIOChnl()->flush();
            // Perform write through.
            
            
            for (i = 0; i < 8; i++) {
                uint8_t pinStateRev =  chnl['H']['i']->read_pin(i); 
                
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
    
 
    
}
