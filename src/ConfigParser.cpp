#include "ConfigParser.h"



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
std::string strip_comments(const std::string  & input, const std::string& delimiters)
{  
    std::string inp = input;
           inp.erase(remove_if(inp.begin(), inp.end(), ::isspace), inp.end());
   return strip_white(inp.substr(0, inp.find_first_of(delimiters)));
}



/*
 * loadConfigFile
 * Loads logic from given filename, and returns it as vector of strings.
 */
std::vector<std::string>  loadConfigfile(std::string filename, bool emptyLines, bool addLineTermination){
    
    if(file_exist(path_prio1+""+filename)){
        filename = path_prio1+""+filename;
        std::cout << "### Using path prio1" << filename << std::endl;
    }
    else if(file_exist(path_prio2+""+filename)){
        filename = path_prio2+""+filename;
        std::cout << "### Using path prio2" << filename << std::endl;
    }
    else{
        throw std::invalid_argument("Error: File not exist: "+filename+" ");
    }
    
    
    std::string                 line;
    std::vector<std::string>    logic;
    std::ifstream               data(filename);
    
    if(!data.is_open()){
       throw std::invalid_argument("Error: Cant open file: "+filename+" Insufficent permission?");
    }    

    while (std::getline(data, line)){
        std::string delimiters("#;");
        line = strip_comments(line, delimiters);
        if(emptyLines || line.size() > 0){ // Test: If empty lines are present, we can tell what line# it was on error.
            // TODO Check Syntax
          
            if(addLineTermination){
                line += ';';
            }
            logic.push_back(line);
        }
    }

    if(logic.size() == 0){
        throw std::invalid_argument("Error: Config file empty!");
    }
   
    return logic;
}

