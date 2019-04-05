
#ifndef CONFIG_PARSER
#define CONFIG_PARSER

#include <iostream> // cout
#include <string>   // std::string
#include <fstream>  // ifstream 
#include <boost/algorithm/string/classification.hpp> // Include boost::for is_any_of
#include <boost/algorithm/string/split.hpp> // Include for boost::split
#include <sys/stat.h> // Check if file exists.
#include <vector>
#include <map>
#include <stdexcept>


#include "ChannelEntitys/Channel_Entity.h"

namespace Config{
static const std::string NO_TOKEN = "";
}


// Paths for config files
static const std::string                 path_prio1 = "./conf/";
static const std::string                 path_prio2 = "/opt/controlpi/";



/*
 * stripWhite
 * Stripes any whitespace from given string.
 * (used for reading configuration)
 */     
std::string strip_white(const std::string& input);

 
/*
 * strip_comments
 * Strips anything starting from the first position of given delimiter.
 * e.g. "#" for Comments.
 */
std::string strip_comments(const std::string  & input, const std::string& delimiters);



inline bool file_exist (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

/*
 * loadConfigFile
 * Loads logic from given filename, and returns it as vector of strings.
 */
std::vector<std::string>  loadConfigfile(std::string filename, bool emptyLines, bool addLineTermination = true);


class EntityDetails{
public:
    int perm_read;
    int perm_write;
    int entityKind;
    
//    static const int ENTITY_INPUT = 1;
//    static const int ENTITY_OUTPUT = 2;
//    static const int ENTITY_DUPLEX = 3;
//    static const int ENTITY_ERROR = -1;
     
    EntityDetails(){
        perm_read  = Channel_Entity::exp_none;
        perm_write = Channel_Entity::exp_none; 
        entityKind = Channel_Entity::ENTITY_ERROR;
    }
    
    EntityDetails(int _entityKind) : EntityDetails(){
        entityKind = _entityKind;
    }
    
   
    
    
};

class configEntity{
public: 
    static const int ENTITY_PIPE        = 1;
    static const int ENTITY_TIMER       = 2;
    static const int ENTITY_HARDWARE    = 3;
    static const int ENTITY_MEMORY      = 4;
    static const int ENTITY_ERROR       = -1;
    int entity_type = ENTITY_ERROR;
    char entityKey = ' ';
    
    std::string private_token = Config::NO_TOKEN; //IO_Channel::NO_TOKEN;
    
    std::map<char, EntityDetails*> entity_detail;
    
    virtual void print(){
    
    }
    
};



class configEntityPipe : public configEntity{
public:
    void print() override{
        std::cout << "Entity " << entityKey     << std::endl;
        std::cout << "token "  << private_token << std::endl;
        std::cout << "entityType" << entity_type << std::endl;
        
    }
    
    //std::string private_token = "-1";
    
};



class configEntityHardware : public configEntity{
public:
    void print() override{
        std::cout << "Entity " << entityKey     << std::endl;
        std::cout << "token "  << private_token << std::endl;
        std::cout << "entityType" << entity_type << std::endl;
        
    }
    
    //std::string private_token = "-1";    
};


class configEntityTimer : public configEntity{
public:
    void print() override{
        std::cout << "Entity " << entityKey     << std::endl;
        std::cout << "token "  << private_token << std::endl;
        std::cout << "entityType" << entity_type << std::endl;
        
    }
    
    //std::string private_token = "-1";    
};


class configEntityMemory : public configEntity{
public:
    void print() override{
        std::cout << "Entity " << entityKey     << std::endl;
        std::cout << "token "  << private_token << std::endl;
        std::cout << "entityType" << entity_type << std::endl;
        
    }
    
    //std::string private_token = "-1";    
};



class globalConf{
public:
    static const int CONTEXT_PIPE        = 1;
    static const int CONTEXT_TIMER       = 2;
    static const int CONTEXT_HARDWARE    = 3;
    static const int CONTEXT_MEMORY      = 4;
    static const int CONTEXT_ERROR       = -1;
private:
    
    int         context    = CONTEXT_ERROR;
    char        instance   = CONTEXT_ERROR; 
    char        entity     = CONTEXT_ERROR;
    int         cfglineNr = 0;
    
    std::string delimiter   = "=";
    size_t      found;
    
    
    
public:
    
    std::map<char, configEntity*> confEnties;
    
   
    
    void print(){
        std::cout << "Print here :-) " << std::endl;
          for(auto const& x  : confEnties){
              std::cout << " Conf identyfier: " << x.first << std::endl;
              x.second->print();
        }
    }
    
    void parseConf(std::string filename){
        std::vector<std::string>  globalConf = loadConfigfile("ControlPi.conf", true, false);
        configEntity* currentContext;
        
        configEntityPipe*     entity_pipe;
        configEntityHardware* entity_hardware;
        configEntityMemory*   entity_memory;
        configEntityTimer*    entity_timer;
        configEntity*         allg;
        
        for(std::string& ln : globalConf){
            cfglineNr++;
            std::cout << "#" << cfglineNr << " " << ln << std::endl;
            std::string lowerLine = ln;
            std::transform(lowerLine.begin(), lowerLine.end(), lowerLine.begin(), ::tolower);
            
            if ((found = ln.find('=')) != std::string::npos){ 
                std::string key    = lowerLine.substr(0,found);
                std::string valueL  = lowerLine.substr(found+1, std::string::npos);
                std::string valueO =        ln.substr(found+1, std::string::npos); 
                
                
                // Switch context (remains until switched again!)
                if(key == "context"){
                    if(valueL == "pipe"){
                        context = CONTEXT_PIPE;
                    }
                    else if(valueL =="hardware"){
                        context = CONTEXT_HARDWARE;
                    }
                    else if(valueL =="memory"){
                        context = CONTEXT_MEMORY;
                    }
                    else if(valueL =="timer"){
                        context = CONTEXT_TIMER;
                    }
                    else{
                        throw std::invalid_argument("Err: Unknown context "+valueL);
                        // Err unknown context
                    }
                }
                
                if(key == "instancekey"){
                    if(valueL.length() != 1){
                       throw std::invalid_argument("Err: Instance key cannot have more than one character "+valueL);
                    }
                    instance = valueO[0]; 
                }
              
                
                // Specific properties.
                if(context == CONTEXT_PIPE){
                    if(key == "instancekey"){
                        confEnties.insert(std::make_pair(instance, (configEntity*) new configEntityPipe));
                        allg = confEnties[instance];
                        allg->entityKey= instance;
                        allg->entity_type =  context;
                        
                        entity_pipe = (configEntityPipe*) confEnties[instance];
                        entity_pipe->entity_type = configEntity::ENTITY_PIPE;
                    }
                                      
                    if(key == "private_token"){
                        entity_pipe->private_token = valueL;
                    }  
                }
                
                
                else if(context == CONTEXT_HARDWARE){
                    
                    if(key == "instancekey"){ 
                       confEnties.insert(std::make_pair(instance, (configEntity*) new configEntityHardware));  
                       allg = confEnties[instance];
                       allg->entityKey= instance;
                       allg->entity_type =  context;
                       entity_hardware = (configEntityHardware*) confEnties[instance];
                       
                    }
                    // Here comes specific settings for HW
                }
                
                else if(context == CONTEXT_MEMORY){
                    if(key == "instancekey"){ 
                       confEnties.insert(std::make_pair(instance, (configEntity*) new configEntityMemory));  
                       allg = confEnties[instance];
                       allg->entityKey= instance;
                       allg->entity_type =  context;
                       entity_memory = (configEntityMemory*) confEnties[instance];
                    }
                    // Here comes specific settings for HW
                }
                
                
                else if(context == CONTEXT_TIMER){
                    if(key == "instancekey"){ 
                       confEnties.insert(std::make_pair(instance, (configEntity*) new configEntityTimer));  
                       allg = confEnties[instance];
                       allg->entityKey= instance;
                       allg->entity_type =  context;
                       entity_timer = (configEntityTimer*) confEnties[instance];
                    }
                    // Here comes specific settings for timer
                }
                
                
                // Unspecific properties 
                if(context != CONTEXT_ERROR && instance != CONTEXT_ERROR){
                
                    if(key == "inputentitykey"){
                        entity = valueO[0];
                        allg->entity_detail.insert(std::make_pair(entity, new EntityDetails(Channel_Entity::ENTITY_INPUT)));
                        //allg->entity_detail[entity]->perm_write = Channel_Entity::exp_none;
                        //allg->entity_detail[entity]->perm_read  = Channel_Entity::exp_none;
                    }
                    
                    if(key == "outputentitykey"){
                        entity = valueO[0];
                        allg->entity_detail.insert(std::make_pair(entity, new EntityDetails(Channel_Entity::ENTITY_OUTPUT)));
                        //allg->entity_detail[entity]->perm_write = Channel_Entity::exp_none;
                        //allg->entity_detail[entity]->perm_read  = Channel_Entity::exp_none;                                
                    }
                    if(key == "duplexentitykey"){
                        entity = valueO[0];
                        allg->entity_detail.insert(std::make_pair(entity, new EntityDetails(Channel_Entity::ENTITY_DUPLEX)));
                        //allg->entity_detail[entity]->perm_write = Channel_Entity::exp_none;
                        //allg->entity_detail[entity]->perm_read  = Channel_Entity::exp_none;                                
                    }
                    
                    
                    if(key == "expose_read"){
                       if(entity == CONTEXT_ERROR){
                           std::string err_msg = "Err Line "+std::to_string(cfglineNr)+": Entity key is not set properly "+entity;
                           throw std::invalid_argument(err_msg);
                       }
                       if(valueL == "public"){
                           allg->entity_detail[entity]->perm_read = Channel_Entity::exp_public;
                       } 
                       else if(valueL == "private"){
                           allg->entity_detail[entity]->perm_read = Channel_Entity::exp_private;
                       }
                       else{
                           allg->entity_detail[entity]->perm_read = Channel_Entity::exp_none;
                       }
                    }
                    
                    if(key == "expose_write"){
                       if(entity == CONTEXT_ERROR){
                           std::string err_msg = "Err Line "+ std::to_string(cfglineNr)+": Entity key is not set properly "+entity;
                           throw std::invalid_argument(err_msg);
                       }
                       if(valueL == "public"){
                           allg->entity_detail[entity]->perm_write = Channel_Entity::exp_public;
                       } 
                       else if(valueL == "private"){
                           allg->entity_detail[entity]->perm_write = Channel_Entity::exp_private;
                       }
                       else{
                           allg->entity_detail[entity]->perm_write = Channel_Entity::exp_none;
                       }
                    }
                                          
                }
                
                
                
            }
            else{
                // Empty or invalid line. Do something? 
            }
        }
    }
    
    // Construct
    globalConf(std::string filename){
        parseConf(filename);
    }
    
    virtual ~globalConf(){
        // renove config objects.
        for(auto const& x  : confEnties){
            for(auto const& z  : x.second->entity_detail){
                delete z.second;
            }
            delete x.second;
            
        }
        
   
    } 
    
};




#endif