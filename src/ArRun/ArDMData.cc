// TV - 29/04/2010 - ArDMData.cc
//
// Library file readout
//

#include <string>
#include "ArDMData.hh"
#include <stdio.h>
#include <errno.h>
#include <iostream>
#include <iostream>
#include <fstream>
using namespace std;

#include "TStopwatch.h"

const int ArDMData_verbose = 0;


/*
void reformatstr(char * in, char * out)
{
  while 
}
*/

////  ArDMConfigFile   Class implementation

  ArDMConfigFile::ArDMConfigFile(const char * filename)
  {
    strcpy(m_paramFileName , filename);
  }

  int ArDMConfigFile::Open()
  {
   int ret=0;
   file = new std::ifstream(m_paramFileName);
   
   if (file->is_open())
   {
       file->close();
       file->open(m_paramFileName);
       ret = 1;
   }
   if (file->is_open())
   {
     ret= 1;
   }
   return ret;
  }

  long   ArDMConfigFile::readData(long * pdata)
  {
    long size[4];
    file->read((char *) size,3*4);
    file->seekg(-3*4,ios::cur);
    file->read((char *) pdata , (size[2]+3)*4);
    return (size[2]+3) ;
  }

#define C_LEN_TO_READ 12
  long   ArDMConfigFile::readAndDoIndex(char type, long runID,tIndexItem *evt)
  {
    long data[C_LEN_TO_READ];
    long size;
    evt->position = file->tellg();
    file->read((char *) data,C_LEN_TO_READ*4);
    if (!file->fail())
    {
      size = data[2];
      evt->runID=0;
      evt->runID = runID;
      evt->type = type;
      evt->crateID = (data[6]>>8)&0xff;
      switch (evt->type)
      {
          case C_PMT_PRODUCER :
                    evt->TTT =   *(data+10);
                    break;
          case C_CHARGE_PRODUCER :
                    evt->TTT =   *(data+8);
                    break;
      }

      file->seekg((size-C_LEN_TO_READ + 3 )*4  ,ios::cur); // 3 : nb long (_PM_, 0 , len of event ) 

/*
      std::cout << "reading index file :" << evt->position << " /  "
                                         << evt->TTT      << " /  "
                                         << data[5]      << " /  "
                                         << evt->runID    << " /  "
                                         << evt->crateID  << " /  "
                                         << evt->type     << " " << std::endl;
*/
      return size;
    }
    return 0;
  }

  void ArDMConfigFile::Close()
  {
     file->close();
  }

  int ArDMConfigFile::findStr(const char * section)
  {
     bool ok = false;
     char wordToFind[1024];
     int n;
     if (!file->is_open()) file->open(m_paramFileName);
     sprintf(wordToFind,"%s",section);
     m_posSection = 0;
     file->seekg(0,ios::beg);
     while ((!file->eof()) && (!ok))
     {
       std::string line;  // déclaration d'une chaîne qui contiendra la ligne lue
       getline(*file, line);  // on met dans "contenu" la ligne

       if ((n=line.find(wordToFind))!=string::npos)
       {
        int pos = (int) file->tellg()-(line.length()-n+1);
        file->seekg(pos,ios::beg);
        m_posSection = file->tellg();
        ok=true;
        return 1;
       }
     }
     return 0;
  }

  int ArDMConfigFile::SetSection(const char * section)
  {
     bool ok = false;
     char wordToFind[1024];
     if (!file->is_open()) file->open(m_paramFileName);
     sprintf(wordToFind,"[%s]",section);
     m_posSection = 0;
     file->seekg(0,ios::beg);
     while ((!file->eof()) && (!ok))
     {
       std::string line;  // déclaration d'une chaîne qui contiendra la ligne lue
       getline(*file, line);  // on met dans "contenu" la ligne
       size_t end = line.find(C_IDENT_CHARGE_READOUT);
       if (end != std::string::npos)  break;
       end = line.find(C_IDENT_CHARGE_READOUT_V101);
       if (end != std::string::npos)  break;
       end = line.find(C_IDENT_PM_READOUT);
       if (end != std::string::npos)  break;

       if (line.find(wordToFind)!=string::npos)
       {
        m_posSection = file->tellg();
//printf("Section:%s pos:0x%x\n",section,m_posSection);
        ok=true;
        return 1;
       }
     }
     return 0;
  }

  char * ArDMConfigFile::GetChar(const char * word,const char * defaultValue)
  {
     bool ok = false;
     bool bsection = false;
     char wordToFind[1024];
     sprintf(wordToFind,"%s = ",word);
     file->seekg(m_posSection,ios::beg);
     while ((!file->eof()) || (!ok))
     {
       std::string line;  // déclaration d'une chaîne qui contiendra la ligne lue
       getline(*file, line);  // on met dans "contenu" la ligne
       size_t end = line.find(C_IDENT_CHARGE_READOUT);
       if (end != std::string::npos)  break;
       end = line.find(C_IDENT_CHARGE_READOUT_V101);
       if (end != std::string::npos)  break;
       end = line.find(C_IDENT_PM_READOUT);
       if (end != std::string::npos)  break;
//       if (line.find(section)!=string::npos) bsection = true;
//printf("c:%d %s(%d) - ",c,word,bsection);
//std::cout<<line<<std::endl;
//       if (bsection)
       {
           if (line.find(wordToFind)!=string::npos)
           {
            char tmp[1024];
            strcpy(tmp,line.c_str()+strlen(wordToFind));
            ok=true;
            return (char *) (line.c_str()+strlen(wordToFind));
           }
       }
     }
//std::cout<<"NOT FIND("<<word<<"):"<<defaultValue<<std::endl;
     return (char *) defaultValue;
  }

  int ArDMConfigFile::Get(const char * word,long  defaultValue)
  {
     bool ok = false;
     bool bsection = false;
  bool b_idrun=false;
     char wordToFind[1024];
     if (!file->is_open()) file->open(m_paramFileName);
     file->seekg(0,ios::beg);
     sprintf(wordToFind,"%s = ",word);
     if (strcmp(word,"ID_RUN")==0) 
     {
       b_idrun=true;
       sprintf(wordToFind,"%s",word);
     }
//printf("Word to file:*%s*\n",wordToFind);
//printf("pos section :0x%x\n",m_posSection);
     file->seekg(m_posSection,ios::beg);
     int m_currPos= m_posSection;
     while ((!file->eof()) || (!ok))
     {
       std::string line;  
       getline(*file, line); 
//printf("ToFind:*%s* word:*%s* l:%d p:0x%x\n",wordToFind,line.c_str(),line.length(),m_currPos);getchar();
       if (line.find(wordToFind)!=string::npos)
       {
            char tmp[1024];
            /// ONLY FOR ID_RUN !!!!!  
            
            if (b_idrun) 
            {
              strcpy(tmp,line.c_str()+4+10);  // 4 to go to ID_RUN string + 10 to go to the value !!!
              file->close();
            }
            else
              strcpy(tmp,line.c_str()+strlen(wordToFind));
            if (strncmp(tmp,"0x",2)==0)
            {
               uint32_t val;
               sscanf(tmp,"%x",&val);
               return val;
            }
            ok=true;
            return atoi(tmp);
       }
       size_t end = line.find(C_IDENT_CHARGE_READOUT);
       if (end != std::string::npos) break;
       end = line.find(C_IDENT_CHARGE_READOUT_V101);
       if (end != std::string::npos) break;
       end = line.find(C_IDENT_PM_READOUT);
       if (end != std::string::npos) break;
     }
     file->close();
//std::cout<<"NOT FIND("<<word<<"):"<<defaultValue<<std::endl;
     return defaultValue;
  }


  int ArDMConfigFile::Get(const char * section,const char * word,long  defaultValue)
  {
     bool ok = false;
     bool bsection = false;
     char wordToFind[1024];
     if (!file->is_open()) file->open(m_paramFileName);
     sprintf(wordToFind,"%s = ",word);
     file->seekg(0,ios::beg);
//printf("getting:%s in %s\n",word,section);
     while ((!file->eof()) && (!ok))
     {
       std::string line;  // déclaration d'une chaîne qui contiendra la ligne lue
       getline(*file, line);  // on met dans "contenu" la ligne
       size_t end = line.find(C_IDENT_CHARGE_READOUT);
       if (end != std::string::npos)  break;
       end = line.find(C_IDENT_CHARGE_READOUT_V101);
       if (end != std::string::npos)  break;
       end = line.find(C_IDENT_PM_READOUT);
       if (end != std::string::npos)  break;
       if (line.find(section)!=string::npos) bsection = true;
       if (bsection)
       {
           if (line.find(wordToFind)!=string::npos)
           {
            char tmp[1024];
            strcpy(tmp,line.c_str()+strlen(wordToFind));
            if (strncmp(tmp,"0x",2)==0)
            {
               uint32_t val;
               sscanf(tmp,"%x",&val);
               return val;
            }
            ok=true;
            return atoi(line.c_str()+strlen(wordToFind));
           }
       }
     }
     return defaultValue;
  }

  ArDMIndexFiles::ArDMIndexFiles(const std::string & dirFiles)
  {
    m_path = dirFiles;
    m_files = vector<string>();
  }

  ArDMIndexFiles::~ArDMIndexFiles()
  {
  }

  std::vector<std::string>  ArDMIndexFiles::readIndexFileList(void)
  {
    DIR *dp;
    struct dirent *dirp;
    if (((dp  = opendir(m_path.c_str()))) != NULL)
    {
       m_files.clear();
       while ((dirp = readdir(dp)) != NULL)
       {
         if (strcmp(dirp->d_name+(strlen(dirp->d_name)-5),"index")==0)
         {
//           std::string label = getRunID((char *) dirp->d_name);
//           m_files.push_back(label);
           m_files.push_back(string(dirp->d_name));
         }
       }
       std::sort( m_files.begin(), m_files.end() );
       closedir(dp);
    }
    return m_files;
  }

  std::string  ArDMIndexFiles::getRunID(char *idRun)
  {
    char tmp[256];
    int index;
    strcpy(tmp,strchr(idRun,'_')+1);
    *(strrchr(tmp,'_'))=0;
    index = atoi(tmp);
    sprintf(tmp,"%d",index);
    
    return (std::string) tmp;
  }


////  ArDMData   Class implementation


ArDMData::ArDMData(const std::string & dirFiles,const std::string & runNumber)
{
  char tmp[1024];
  char *endptr;
  errno = 0;    /* To distinguish success/failure after call */
  long val = strtol(runNumber.c_str(), &endptr, 10);

    /* Check for various possible errors */

    if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
                   || (errno != 0 && val == 0) 
                   || (endptr == runNumber.c_str())) 
    {
               printf("ERROR:  run number not correct: %s\n",runNumber.c_str());
               exit(EXIT_FAILURE);
    }
  sprintf(tmp,"%06lu_00",val);
  m_name = dirFiles+"/run_"+tmp+".index";
printf("Name:%s\n",m_name.c_str());
  m_dir=dirFiles;
  m_runNumber=runNumber;
  indexFile.open(m_name.c_str());
  if (!indexFile.is_open()) 
    {
               printf("ERROR:  file doesn't exist : %s\n",m_name.c_str());
               exit(EXIT_FAILURE);
    }
    m_currentFileName[0]=0;
    m_idEvent = 0;
    m_lastPositionInIndexFile=0;
    m_lastEventFlag=0;
    m_lastTTT=0;
    m_indexFilesConsistent = this->readParams();

  }

  ArDMData::~ArDMData()
  {
    indexFile.close();
  }

  int  ArDMData::checkIndexFile()
  {
     int crate = 0;
     int maskOrig = this->getBoardMask(crate);
     int mask = maskOrig;
     int delta;
     bool first = true;
     long TTT2Compare = 0;
     //Init 
     if (this->isDataReadable())
     {
        for (int i=0;i<C_NBMAX_PMT_BOARD;i++)
        {
            m_checkDAQ[i].set = false;
            m_checkDAQ[i].TTT = 0;
        }
        // Find first event for each board
        for (int i=0;i<C_NBMAX_PMT_BOARD;i++)
        {
           if ((mask>>i)&0x01)
           {
              int idEvent=0;
              tIndexItem evt;
              bool ok = false;
              do 
              {
                 if (!this->readSubEvent(idEvent,&evt,false)) return -1;
                 if ((!m_checkDAQ[i].set) && (i==this->getBoard()))
                 {
                    m_checkDAQ[i].TTT = this->getTTT();
                    m_checkDAQ[i].set = true;
                    m_checkDAQ[i].idEvent = idEvent;
                 }
                 idEvent++;
              } while (!m_checkDAQ[i].set);
           }
        }

        // check the difference between each TTT of each Board
        // if delta < C_TOL              -->  Boards start with S_IN controlled by run mode
        // if delta ~ couple of thousand -->  Boards start by software 
        delta = 0;
        first = true;
        TTT2Compare = 0;
        for (int i=0;i<C_NBMAX_PMT_BOARD;i++)
        {
           if ((mask>>i)&0x01)
           {
              if (first) 
              {
                  TTT2Compare = m_checkDAQ[i].TTT;
                  first = false;
              }
              else
              {
                 if (delta < abs(TTT2Compare-m_checkDAQ[i].TTT) )
                 {
                     delta = abs(TTT2Compare-m_checkDAQ[i].TTT);
                 }
              }
           }
           printf("board:%d Event:%d TTT:%d \n",i,m_checkDAQ[i].idEvent,m_checkDAQ[i].TTT);
        }
     } 
     printf("delta:%d \n",delta);
     return delta;
  }

  bool ArDMData::isDataReadable()
  {
    return m_indexFilesConsistent;
  }

  int  ArDMData::readPos(tIndexItem *evt,int idSubEvent)
  { 
//printf("idEvent:%d\n",idEvent);getchar();
    indexFile.seekg (0, ios::end);
    uint32_t length = indexFile.tellg();
    int lastEvent=length/sizeof(tIndexItem)-1;
    if (length<sizeof(tIndexItem)) return -1;
    if (idSubEvent>lastEvent) return -1;

    indexFile.seekg (idSubEvent*sizeof(tIndexItem), ios::beg);
    if (! indexFile.fail())
    {
       indexFile.read ((char *) evt, sizeof(tIndexItem));
    
   /* 
       std::cout << "reading index file :"<< "IdEvent:"<<idEvent << " / "
                                          << evt->position << " /  "
                                          << evt->TTT      << " /  "
                                          << evt->runID    << " /  "
                                          << evt->crateID  << " /  "
                                          << evt->type     << " " << std::endl;
   */ 
       return 1 ;
     } 
    return 0 ;
  }


  int  ArDMData::readLastPos(tIndexItem *evt)
  {
    indexFile.seekg (0, ios::end);
    uint32_t length = indexFile.tellg();
    m_nbEvent = length/sizeof(tIndexItem)-1;
    indexFile.seekg (m_nbEvent*sizeof(tIndexItem), ios::beg);
    indexFile.read ((char *) evt, sizeof(tIndexItem));
    /*
    std::cout << "last position :"<< "IdEvent:"<<m_nbEvent << " / "
                                       << evt->position << " /  "
                                       << evt->TTT      << " /  "
                                       << evt->runID    << " /  "
                                       << evt->crateID  << " /  "
                                       << evt->type     << " " << std::endl;
    */
    return m_nbEvent ;
  }

  long *  ArDMData::readSubEvent(int idEvent,bool last)
  {
    tIndexItem evt ;
    return this->readSubEvent(idEvent,&evt,last);
  }

  long *  ArDMData::readSubEvent(int idEvent,tIndexItem *evt,bool last)
  {
//    tIndexItem evt ;
   
    m_idEvent = idEvent;
    if (!last) 
    {
      if (!this->readPos(evt,idEvent))
      {
        return NULL;
      }
    }
    else       m_idEvent=this->readLastPos(evt);
    return readBlock(evt);
  }

  long *  ArDMData::readBlock(tIndexItem *evt)
  {

    TStopwatch timer;
    timer.Start();

    if(ArDMData_verbose) cout<<"in ArDMData::readBlock(..) ... checking current filename "<<endl;


    char name[1024];
    char filename[1024];
    long buffer[4];

    sprintf(name,"run_%06d_%02d_%s.raw",evt->runID,evt->crateID,getFileTypeProdLabel(evt->type));
    sprintf(filename,"%s/%s",m_dir.c_str(),name);
//printf("Name:%s\n",filename);
    m_idrun = evt->runID;
    m_currentFile[evt->crateID] = name;
    if (strcmp(filename,m_currentFileName)!=0)
    {
       strcpy(m_currentFileName,filename);
       if (datafile.is_open()) datafile.close();
       datafile.open(m_currentFileName, std::ios::binary);
       if (!datafile.is_open())
       {
               printf("ERROR:  Problem to open file : %s\n",m_currentFileName);
               exit(EXIT_FAILURE);
       }
    }




  if(ArDMData_verbose){
    timer.Stop();
    cout<<"in ArDMData::readBlock(..) ...  finished loading data from rawDataFile ... \t\t\t\t"<<timer.RealTime()<<endl;
    timer.Start();
  }




    if(ArDMData_verbose) cout<<"in ArDMData::readBlock(..) ... calling datafile.read(...) ... "<<endl;


    //printf("Reading raw data file:%s to position:%d\n",m_currentFileName,evt.position);
    memset(buffer,0,4);
    datafile.seekg(evt->position,ios_base::beg);
    datafile.read((char *) buffer, 3*sizeof(long));
    memset(buffData,0,buffer[2]*sizeof(long));

    datafile.read((char *) buffData,buffer[2]*sizeof(long));




    if(ArDMData_verbose){
      timer.Stop();
      cout<<"in ArDMData::readBlock(..) ...  finished datafile.read(...) ... \t\t\t\t"<<timer.RealTime()<<endl;
      timer.Start();
    }





    if(ArDMData_verbose) cout<<"in ArDMData::readBlock(..) ...  storing data to buffer ... "<<endl;



    m_key         =   *(buffData+0);
    m_sizeTTEvent =   *(buffData+1);
    m_timeStamp   =   *(buffData+2);
    m_crate       =  (*(buffData+3) >> 8 )&0x0f;
    m_board       =   *(buffData+3)&0x0f;
    switch (evt->type) 
    {
        // Light
        case C_PMT_PRODUCER : 
                  m_sizeData    = ((*(buffData+4)&0x7fff) -4)*2;  // Nb sample (without header)
                  m_channel     = *(buffData+5)&0xff;
//                  m_TTT         =   *(buffData+7)&0x7fffffff;
                  m_TTT         =   *(buffData+7);
//                  m_sizeData=(m_sizeTTEvent-6)*2;
                  m_ptData      =   buffData+8;
                  m_eventID     = *(buffData+6)&0xffffff;
                  break;
        // Charge
        case C_CHARGE_PRODUCER : 
                  m_flags       = ((*(buffData+4)&0xe0000000)>>29);
                  m_channel     = ((*(buffData+4)&0x1f000000)>>24);
                  m_lostTrigger = ((*(buffData+4)&0x00FF0000)>>16);
                  m_sizeData    = ((*(buffData+4)&0x7fff) -2)*2;  // Nb sample (without header)
                  m_TTT         =   *(buffData+5);
                  m_ptData      =   buffData+6;
                  break;
    }


    if(ArDMData_verbose){
      timer.Stop();
      cout<<"in ArDMData::readBlock(..) ...  finished storing data to buffer ... \t\t\t\t"<<timer.RealTime()<<endl;
      timer.Start();
   }

    timer.Stop();

    if (evt->TTT!=m_TTT) printf("ERROR index TTT:%u Raw data TTT:%u\n",evt->TTT,m_TTT);
             // return NULL;

//printf("evt.type:%d size:%d key: 0x%x TS:%u size:%d TTT:%u\n",evt->type,buffer[2]*sizeof(long),m_key,m_timeStamp, m_sizeData,m_TTT);

    return buffData ; 
  }

  int ArDMData::getSubEvent(int eventID,int board,tIndexItem *evt)
  {
    indexFile.seekg (0, ios::end);
    uint32_t length = indexFile.tellg();
    m_nbSubEvent = length/sizeof(tIndexItem)-1;
    int idSubEvent=0;    
    bool ok;
    do
    {
         indexFile.seekg (idSubEvent*sizeof(tIndexItem), ios::beg);
         if (! indexFile.fail())
         {
             indexFile.read ((char *) evt, sizeof(tIndexItem));
             if ((eventID == evt->eventID) && (board==evt->board))
             {
                if (!this->readSubEvent(idSubEvent,evt,0)) return -1;
                return 1;
             }
         }
         else return -1; //EOF
         idSubEvent++;
    } while ( 1 );
  }


int ArDMData::getEvent(int eventID,tIndexItem evt[]){
  
  //return getEvent_checkEventNumber(eventID,evt);
  return getEvent_checkTTT(eventID,evt);
  //return getEvent_checkEventNumber_thierry(eventID,evt);

}




int ArDMData::getEvent_checkTTT(int eventID,tIndexItem evt[])//khoi's version 2 //check the TTT
  { 


    tIndexItem currEvt;
    int mask=0;


    if(ArDMData_verbose) cout<<"\n\nin ArDMData::getEvent_checkTTT(..) .... start new event .... "<<endl;
    
    if(0&& ArDMData_verbose) 
      cout<<"i'm looking for event "<<eventID
	  <<"\t m_lastTTT "<<m_lastTTT
	  <<"\t m_lastPositionInIndexFile "<<m_lastPositionInIndexFile
	  <<endl;


    do{
      indexFile.seekg(m_lastPositionInIndexFile*sizeof(tIndexItem),ios::beg);
      if(indexFile.fail()) return mask;
      indexFile.read((char*)&currEvt,sizeof(tIndexItem));

	
      if(ArDMData_verbose)
	cout<<"position 1 ..."
	    <<"m_lastPositionInIndexFile "<<m_lastPositionInIndexFile
	    <<"\t m_lastTTT "<<m_lastTTT
	    <<"\t currTTT "<<currEvt.TTT
	    <<"\t board "<<currEvt.board
	    <<"\t eventID "<<currEvt.eventID
	    <<endl;
      
	
      if(m_lastTTT==0){
	if(eventID == currEvt.eventID){
	  memcpy(&evt[currEvt.board],&currEvt,sizeof(tIndexItem));
	  mask |= 1<<currEvt.board;
	  m_lastTTT = currEvt.TTT;
	  if(ArDMData_verbose) cout<<"get first TTT "<<currEvt.TTT<<"....."<<endl;
	}      
	m_lastPositionInIndexFile++;	
      }else{
	uint32_t currTTT = currEvt.TTT;
	if(abs((long int)(currTTT - m_lastTTT)) < C_TTT_MARGIN_WITHIN_ONE_EVENT){
	  //read data
	  memcpy(&evt[currEvt.board],&currEvt,sizeof(tIndexItem));
	  mask |= 1<<currEvt.board;
	  m_lastPositionInIndexFile++;
	  m_lastTTT = currTTT;	    
	  if(ArDMData_verbose) cout<<"\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t reading data ... "<<endl;
	}else{
	  if(ArDMData_verbose) cout<<"currTTT - m_lastTTT > "<<C_TTT_MARGIN_WITHIN_ONE_EVENT
		    <<"\t m_lastPositionInIndexFile "<<m_lastPositionInIndexFile
		    <<"\t ... break do-while loop. this block will be re-processed in next event."
		    <<endl;
	  m_lastTTT = currTTT;
	  break;
	}
      }
      
    
    }while(1);




//     if(m_lastTTT==0){
//       //at the very beginning, m_lastTTT is 0
//       //search for the first boardBlock with the event number indicated in eventID
//       do{
// 	indexFile.seekg(m_lastPositionInIndexFile*sizeof(tIndexItem),ios::beg);
// 	if(!indexFile.fail()){
// 	  indexFile.read((char*)&currEvt,sizeof(tIndexItem));
// 	  if(eventID == currEvt.eventID){
// 	    memcpy(&evt[currEvt.board],&currEvt,sizeof(tIndexItem));
// 	    mask |= 1<<currEvt.board;
// 	    m_lastTTT = currEvt.TTT;
// 	  }
	  
// 	  if(ArDMData_verbose)
// 	    cout<<"position 1 ..."
// 		<<"m_lastPositionInIndexFile "<<m_lastPositionInIndexFile
// 		<<"\t m_lastTTT "<<m_lastTTT
// 		<<"\t TTT "<<currEvt.TTT
// 		<<"\t board "<<currEvt.board
// 		<<"\t eventID "<<currEvt.eventID
// 		<<endl;

	  
// 	}else return mask;
	
// 	m_lastPositionInIndexFile++;
//       }while(m_lastTTT==0);


//     }else{
//       do{
// 	indexFile.seekg(m_lastPositionInIndexFile*sizeof(tIndexItem),ios::beg);
// 	if(!indexFile.fail()){
// 	  indexFile.read((char*)&currEvt,sizeof(tIndexItem));
// 	  uint32_t currTTT = currEvt.TTT;
	  
// 	  if(ArDMData_verbose)
// 	    cout<<"position 2 ...."
// 		<<"m_lastPositionInIndexFile "<<m_lastPositionInIndexFile
// 		<<"\t m_lastTTT "<<m_lastTTT
// 		<<"\t TTT "<<currEvt.TTT
// 		<<"\t board "<<currEvt.board
// 		<<"\t eventID "<<currEvt.eventID
// 		<<endl;

// 	  if(abs((long int)(currTTT - m_lastTTT)) < C_TTT_MARGIN_WITHIN_ONE_EVENT){
// 	    //read data
// 	    memcpy(&evt[currEvt.board],&currEvt,sizeof(tIndexItem));
// 	    mask |= 1<<currEvt.board;
// 	    m_lastPositionInIndexFile++;
// 	    m_lastTTT = currTTT;	    
// 	    if(ArDMData_verbose) cout<<"\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t reading data ... "<<endl;
// 	  }else{
// 	    //if currTTT is not within the TTT_margin compared to m_lastTTT
// 	    //get the new TTT
// 	    //don't increment m_lastPositionInIndexFile, so that this block will be re-read in next event !
// 	    m_lastTTT = currTTT;	    
// 	    break;
// 	  }
// 	}else return mask;
      
//       }while(1);
    
    
//     }

    if(0&& ArDMData_verbose){
      cout<<"exit ...."
	  <<"m_lastPositionInIndexFile "<<m_lastPositionInIndexFile
	  <<"\t m_lastTTT "<<m_lastTTT
	  <<"\t TTT "<<currEvt.TTT
	  <<"\t board "<<currEvt.board
	  <<"\t eventID "<<currEvt.eventID
	  <<"\t mask "<<mask
	  <<endl;
    }

    return mask;
  }




int ArDMData::getEvent_checkEventNumber(int eventID,tIndexItem evt[])//khoi's version 1 //check the event number
  { 


    tIndexItem currEvt;
    int mask=0;

    if(0&& ArDMData_verbose) 
      cout<<"i'm looking for event "<<eventID<<"\t m_lastPositionInIndexFile "<<m_lastPositionInIndexFile
	  <<endl;
    do
      {
	if(0&& ArDMData_verbose){
	  
	  cout<<"i'm looking for event "<<eventID<<"\t m_lastPositionInIndexFile "<<m_lastPositionInIndexFile
	      <<"\t position 2 ...."
	      <<endl;//getchar();
	}
	if(m_lastEventFlag) m_lastPositionInIndexFile--; 
	m_lastEventFlag=0;
	//m_lastEventFlag == 1 <--> do-while loop broken because eventID < currEvt.eventID
	//--> re-read the this block again in next event !! --> that's why "m_lastPositionInIndexFile-- "!
	//after using the flag, set it to 0 immediately !!
	


	indexFile.seekg (m_lastPositionInIndexFile*sizeof(tIndexItem), ios::beg);
	if (! indexFile.fail())
	  {
	    indexFile.read ((char *) &currEvt, sizeof(tIndexItem));
	    if(0&& ArDMData_verbose) 
	      cout<<"i'm looking for event "<<eventID
		   <<"\t currEvt.eventID "<<currEvt.eventID
		  <<"\t m_lastPositionInIndexFile "<<m_lastPositionInIndexFile
		  <<endl;
	    if (eventID == currEvt.eventID) 
	      {
		memcpy(&evt[currEvt.board],&currEvt,sizeof(tIndexItem));
		mask |= 1<<currEvt.board;
		if (mask==0xf){
		  m_lastPositionInIndexFile++;
		  m_lastEventFlag=0;
		  if(0&& ArDMData_verbose){
		    cout<<"i'm looking for event "<<eventID
			<<"\t currEvt.eventID "<<currEvt.eventID
			<<"\t m_lastPositionInIndexFile "<<m_lastPositionInIndexFile
			<<"\t mask "<<mask
			<<"\t exit position 1 ... "
			<<endl; //getchar();
		  }

		  return mask;
		}
	      }
	  }
	else return mask;
	m_lastPositionInIndexFile++;
      } while ( eventID>= currEvt.eventID);

    if(0&& ArDMData_verbose){
      cout<<"i'm looking for event "<<eventID
	  <<"\t currEvt.eventID "<<currEvt.eventID
	  <<"\t m_lastPositionInIndexFile "<<m_lastPositionInIndexFile
	  <<"\t mask "<<mask
	  <<"\t exit position 2 ... "
	  <<endl; //getchar();
    }
    m_lastEventFlag=1; //do-while loop broken due to eventID < currEvt.evenID --> re-read this block again in next event !
    return mask;
  }



int ArDMData::getEvent_checkEventNumber_thierry(int eventID,tIndexItem evt[])//Thierry's
  { 

    indexFile.seekg (0, ios::end);
    //uint32_t length = indexFile.tellg();
    //m_nbSubEvent = length/sizeof(tIndexItem)-1;
    int idSubEvent=0;    
    tIndexItem currEvt;
    bool ok;
    int mask=0;


    for (int i=0;i<C_NBMAX_PMT_BOARD;i++)
    {
      if(ArDMData_verbose) 
	cout<<"i'm looking for event "<<eventID<<"\t idSubEvent "<<idSubEvent
	  <<"\t board i "<<i<<"\t position 1 ...."
	  <<endl;
        do
        {
      if(ArDMData_verbose) 
	  cout<<"i'm looking for event "<<eventID<<"\t idSubEvent "<<idSubEvent
	      <<"\t board i "<<i<<"\t position 2 ...."<<endl;//getchar();

           indexFile.seekg (idSubEvent*sizeof(tIndexItem), ios::beg);
           if (! indexFile.fail())
           {
               indexFile.read ((char *) &currEvt, sizeof(tIndexItem));
	       if(ArDMData_verbose) 
		 cout<<"i'm looking for event "<<eventID
		   <<"\t currEvt.eventID "<<currEvt.eventID
		   <<"\t idSubEvent "<<idSubEvent
		   <<"\t board i "<<i
		   <<endl;
               if (eventID == currEvt.eventID) 
               {
                  memcpy(&evt[currEvt.board],&currEvt,sizeof(tIndexItem));
                  mask |= 1<<currEvt.board;
                  if (mask==0xf) return mask;
               }
           }
           else return mask;
           idSubEvent++;
        } while ( 1 );
    }
  }

  float ArDMData::getRealTTT(unsigned long ttt)
  {
    static unsigned long ttt_old =0 ;
    static unsigned long inc =0 ;
    float tttr ;
    if (ttt_old > (ttt  & 0x7fffffff)) 
    {
      inc++;
printf("inc : %u ttt_old:%u \n",inc,ttt_old);
    }
    ttt_old = ttt  & 0x7fffffff;
    tttr = (float)( ( (float) inc* (float) 0x7fffffff) +  (float) (ttt & 0x7fffffff));
    return tttr;
  }

  unsigned long ArDMData::getTimeStamp(void)
  {
    return  m_timeStamp;
  }

uint32_t ArDMData::getTTT(void)
  {
    return m_TTT;
  }

  unsigned int ArDMData::getCrate(void)
  {
    return m_crate;
  }

  unsigned int ArDMData::getBoard(void)
  {
    return m_board;
  }

  unsigned int ArDMData::getNbData(void)
  {
    return m_sizeData;
  }

  unsigned int ArDMData::getEventID(void)
  {
    return m_eventID;
  }

  unsigned int ArDMData::getFlags(void)
  {
    return m_flags;
  }

  unsigned int ArDMData::getNbChannels(void)
  {
    unsigned int n=0;
    for (int i=0;i<32;i++)
    {
       if ((m_channel>>i)&0x1)
       {
         n++;
       }
    }
    return n;
  }

  unsigned int ArDMData::getNbChannels(int crateid,int boardid)
  {
    unsigned int n=0;
    for (int i=0;i<32;i++)
    {
       if ((m_channelMask[crateid][boardid]>>i)&0x1)
       {
         n++;
       }
    }
    return n;
  }

  unsigned int ArDMData::getChannel(void)
  {
    return m_channel;
  }

  unsigned int ArDMData::getLostTrigger(void)
  {
    return m_lostTrigger;
  }

  unsigned long ArDMData::getChannelMask(int crateid,int boardid)
  {
   return m_channelMask[crateid][boardid];
  }

  int ArDMData::getData(unsigned int idData)
  {
    int val;
    if (idData>m_sizeData) return C_DAQ_NO_DATA;

    val = *(m_ptData + idData/2) ; 
    if (idData%2) val >>= 16; 
    
    return val & 0xfff;
  }

  int ArDMData::getLightData(int channelid,unsigned int idData)
  {
    int val;
    if (idData>m_sizeData) return C_DAQ_NO_DATA;

    val = *(m_ptData+channelid*m_sizeData/8/2 + idData/2) ;
    if (idData%2) val >>= 16;
//printf("ch:%d idData:%d : %d\n",channelid,idData,val&0xfff );

    return val & 0x3fff;
  }


  long * ArDMData::getPtData(void)
  {
    return m_ptData;
  }

  unsigned long ArDMData::getThreshold(const char* kind,int crateid,int boardid,int channelid)
  {
   unsigned long val=-2;
   if (strcmp(kind,"LTHRES")==0) val = m_lthres[crateid][boardid][channelid];
   else                          val = m_hthres[crateid][boardid][channelid];
   return val;
  }

  unsigned long ArDMData::getDAC(char kind,int crateid,int boardid)
  {
   return m_dac[(kind=='A')?0:1][crateid][boardid] ;
  }

  unsigned long ArDMData::getPostTrigger(int crateid,int boardid)
  {
   return m_postTrigger[crateid][boardid];
  }

  unsigned long ArDMData::getSamplingRate(void)
  {
   return m_samplingRate;
  }

  unsigned long ArDMData::getIdRun(void)
  {
   return m_idrun;
  }

  unsigned long ArDMData::getRunTimeStamp(void)
  {
    this->readSubEvent(0,false);
    return  m_timeStamp;
  }

  unsigned int  ArDMData::getCrateMask(void)
  {
     return m_maskCrate;
  }

  unsigned int  ArDMData::getNbCrate(void)
  {
     return m_nbCrate;
  }

  unsigned int  ArDMData::getBoardMask(int crate)
  {
     return m_maskBoard[crate];
  }

  unsigned int  ArDMData::getBufferSize(void)
  {
    return m_bufferOrg;
  }

  unsigned long ArDMData::getKey(void)
  {
    return m_key;
  }

  unsigned long ArDMData::getTotalSize(void)
  {
    return m_sizeTTEvent;
  }

  unsigned long ArDMData::getNbEvent(void)
  {
    long length,tmp = indexFile.tellg();
    indexFile.seekg (0, ios::end);
    length = indexFile.tellg();
    m_nbEvent = length/sizeof(tIndexItem);
    indexFile.seekg (tmp, ios::beg);
    return m_nbEvent;
  }

  std::string ArDMData::getCurrentFile(int crateId)
  {
    return m_currentFile[crateId];
  }

  unsigned long ArDMData::getCurrentIdEvent(void)
  {
    return m_idEvent;
  }

  bool ArDMData::readParams(void)
  {
   char tmp[255];
   bool ret = true;

   tIndexItem evt ;

   if (this->readPos(&evt,0)!=1) return ret=false; // Read the first entry 
   sprintf(m_currentParametersFileName,"%s/run_%06d_%02d_%s.raw",m_dir.c_str(),evt.runID,evt.crateID,getFileTypeProdLabel(evt.type));

   ArDMConfigFile *param = new ArDMConfigFile(m_currentParametersFileName);
   if (param->Open())
   {
       param->SetSection("RunControl");
       m_maskCrate = 0;
       int m_runsizelimit = param->Get("RunSizeLimit", 0LL);
       m_idrun = param->Get("ID_RUN", 0LL);
//printf("Id_Run:%d\n",m_idrun);
       long maxEventSize ;
//       std::cout << "m_runsizelimit: " << m_runsizelimit << std::endl;
       maxEventSize = param->Get((char *)((evt.type==C_CHARGE_PRODUCER)?"sizeChargeBuffer":"sizePMTBuffer"), -1);
       printf("maxEventSize:%d\n",maxEventSize);
       //       free (buffData);
       buffData = (long *) malloc((maxEventSize + 100) * sizeof(long) );  // 50 for header
       m_nbCrate = 0; 
       for (unsigned int i=0;i<C_MAX_CRATE;i++)
         {
           sprintf(tmp,"%s_%d",(evt.type==C_CHARGE_PRODUCER)?"CHARGE_CRATE":"PM_MAP_BOARD",i);
           m_maskBoard[i] = param->Get(tmp, -1);
//printf("evt.type:%d file:%s tmp:%s Mask(%d) : 0x%x\n",evt.type,m_currentParametersFileName,tmp,i,m_maskBoard[i]);
           if ((m_maskBoard[i]!=-1) && (m_maskBoard[i]!=0))
           {
             m_maskCrate |= 1<<i;
             m_nbCrate++;
           }
         }
       for (int c=0;c<C_MAX_CRATE;c++)
       {
         char section[50];
         char thres[50];
         float fTmp;
         if ((m_maskCrate>>c)&0x1)
         {
            for (int b=0;b<C_NBMAX_BOARD;b++)
              if ((m_maskBoard[c]>>b)&0x1)
              {
                if (evt.type==C_CHARGE_PRODUCER)
                       sprintf(section,"%s_%d%d","Producer.board",c,b);
                else
                       sprintf(section,"%s_%d","PM.board",b);
                param->SetSection(section);
                m_bufferOrg = param->Get((char *)((evt.type==C_CHARGE_PRODUCER)?"BUFFER_ORG":"PM_BUFFER_ORG"),-1);
                m_postTrigger[c][b] = param->Get((char *)((evt.type==C_CHARGE_PRODUCER)?"POST_TRG":"PM_POST_TRG"),-1);
//TODO for light readout :: all dac should have the own value, not the dac 0 
sprintf(tmp,"PM_DAC%02d",0);
                m_dac[0][c][b] = param->Get((char *)((evt.type==C_CHARGE_PRODUCER)?"DAC_A":tmp),-1);
                m_dac[1][c][b] = param->Get((char *)((evt.type==C_CHARGE_PRODUCER)?"DAC_B":tmp),-1);
                m_channelMask[c][b] = param->Get(section,(char *)((evt.type==C_CHARGE_PRODUCER)?"CH_ENABLE":"PM_CHANNEL"),-1);
                fTmp = 10000000./param->Get("SAMPL_FREQ",-1);
                m_samplingRate = (unsigned long) fTmp;
                for (int ch=0;ch<((evt.type==C_CHARGE_PRODUCER)?C_CHARGE_NBMAX_CHANNEL_PER_BOARD:C_LIGHT_NBMAX_CHANNEL_PER_BOARD);ch++)
                {
                    sprintf(thres,"%s%02d",(evt.type==C_CHARGE_PRODUCER)?"HTHRES":"PM_THRES",ch);
                    m_hthres[c][b][ch] = param->Get(thres,-1);
                   //printf("section:%s c:%d b:%d ch:%d lt:%d ht:%d\n",section,c,b,ch,m_lthres[c][b][ch],m_hthres[c][b][ch]);
                }
                for (int ch=0;ch<((evt.type==C_CHARGE_PRODUCER)?C_CHARGE_NBMAX_CHANNEL_PER_BOARD:C_LIGHT_NBMAX_CHANNEL_PER_BOARD);ch++)
                {
                    sprintf(thres,"%s%02d",(evt.type==C_CHARGE_PRODUCER)?"LTHRES":"PM_THRES",ch);
                    m_lthres[c][b][ch] = param->Get(thres,-1);
                   //printf("section:%s c:%d b:%d ch:%d lt:%d ht:%d\n",section,c,b,ch,m_lthres[c][b][ch],m_hthres[c][b][ch]);
                }

              }
          }
       }
       param->Close();
    }
    else 
    {
      fprintf(stderr,"%s no readable !\n",m_currentParametersFileName);
      ret = false;
    }
    return ret;
  } 


////  ArDMRawDataFile   Class implementation


  ArDMRawDataFile::ArDMRawDataFile (const std::string & dirFiles,const std::string & begNumber,const std::string & endNumber,char type)
  {
    aFile = new ArDMData(dirFiles,begNumber);
    m_dir = dirFiles;
    m_begNumber = atoi(begNumber.c_str());
    m_type = type;
  }

  void ArDMRawDataFile::BuildIndexFile(char * nameFile)
  {
    char name[255],filename[255];
    sprintf(name,"run_%06d_%02d_%s.raw",m_begNumber,0,getFileTypeProdLabel(m_type));
    sprintf(filename,"%s/%s",m_dir.c_str(),name);
printf("File:%s\n",filename);
    ArDMConfigFile *param = new ArDMConfigFile(filename);
    if (param->Open())
    {
       if (param->findStr("_PM_"))
       {
         tIndexItem evt;
         while (param->readAndDoIndex(m_type,m_begNumber,&evt))
         {
            indexfile.push_back(evt);
            printf("TTT:%u \n",evt.TTT);
         }
       }
       else printf("Error\n");
    }
  }
