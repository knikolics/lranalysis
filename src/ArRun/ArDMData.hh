// TV - 29/04/2010 - ArDMData.h
//
// Header of the library file readout
//

#ifndef ARDM_INCLUDED_ArDMData
#define ARDM_INCLUDED_ArDMData

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>
#include <fstream>
#include <vector>
#include <iostream>
#include <dirent.h>

#define C_DIR_BIN   "../bin/"
#define C_DIR_CONF  "../conf/"
#define C_DIR_LOGS  "../logs/"
#define C_FILE_CONF C_DIR_CONF"ArDM.conf" 
#define C_PM_FILE_CONF C_DIR_CONF"ArDM_PM.conf" 

#define C_COLLECTOR_NAME C_DIR_BIN"/ArDMCollector.exe "
#define C_PRODUCER_NAME C_DIR_BIN"/ArDMProducer.exe "

#define C_SOFTWARE_RELEASE_101   0x0101
#define C_SOFTWARE_RELEASE_102   0x0102
#define C_CURRENT_SOFTWARE_RELEASE   C_SOFTWARE_RELEASE_102

#define C_KEY   (0x01230000 | C_CURRENT_SOFTWARE_RELEASE)

#define C_IDENT_DUMMY_READOUT  		"____"
#define C_IDENT_CHARGE_READOUT_V101  	"_DAQ"
#define C_IDENT_CHARGE_READOUT  	"_CH_"
#define C_IDENT_PM_READOUT  		"_PM_"
#define C_IDENT_HEADER_FOOTER  		"_DET"

#define C_END_MARK  -1
#define C_HEADER 6 
#define C_DAQ_NO_DATA   0xFFFF 

#define C_MAX_CRATE 4
#define C_NBMAX_BOARD 8

#define C_NBMAX_PMT_BOARD 4

#define C_NBMAX_CHANNEL_PER_BOARD 32    // to delete
#define C_CHARGE_NBMAX_CHANNEL_PER_BOARD 32
#define C_LIGHT_NBMAX_CHANNEL_PER_BOARD 8

#define C_MAX_BLOCK 500

#define C_CIRCULAR_BUFF_SHARED_MEM      // Circular buffer enable , undef --->  only one buffer

#define C_SORT_LIMIT 2500000

#define C_TOL 10


#define C_TTT_MARGIN_WITHIN_ONE_EVENT 5  //TTT of the boards within 1 event can be off by, say, 2 from each other

typedef enum
{
  C_DUMMY_PRODUCER,
  C_PMT_PRODUCER,
  C_CHARGE_PRODUCER,
} tProducer;


typedef struct TYPE_EVENT
{
   int type;
   char fileLabel[10];  // Label added in the name file (ex: CH in run_003262_00_CH.raw )
   char IdLabel[10];    // ex: _CH_ in the file 
}tTypeEvent;

const  tTypeEvent m_TypeEvent[3] = 
{
  {  C_DUMMY_PRODUCER, "__", C_IDENT_DUMMY_READOUT } ,
  {  C_PMT_PRODUCER   , "PMT", C_IDENT_PM_READOUT } ,
  {  C_CHARGE_PRODUCER, "CH", C_IDENT_CHARGE_READOUT } 
};

#define getFileTypeProdLabel(type)  m_TypeEvent[type].fileLabel
#define getIdentTypeProdLabel(type)  m_TypeEvent[type].IdLabel

typedef struct INDEXITEM
{
  uint32_t position;
  uint32_t TTT;
  uint32_t runID;
  uint32_t crateID;
  uint32_t type;  // 0 : Charge    1 : Light
  uint32_t eventID;
  uint32_t board;
}tIndexItem;


typedef struct tSortData
{
    unsigned long pos;
    unsigned long TTT;
}TSORTDATA;


typedef struct tcheckDAQ
{
 bool   set;
 int    idEvent;
 long   TTT;
}TCHECKDAQ;



class ArDMConfigFile
{
  public:

  ArDMConfigFile(const char * filename);
  int Open();
  long   readData(long * pdata);
  long   readAndDoIndex(char type, long runID,tIndexItem *evt);
  void Close();
  int findStr(const char * section);
  int SetSection(const char * section);
  char * GetChar(const char * word,const char *  defaultValue);
  int Get(const char * word,long  defaultValue);
  int Get(const char * section,const char * word,long  defaultValue);

 public:
  std::ifstream *file;
  char m_paramFileName[1024];
  long m_posSection;
  long buffer[200000];
};


class ArDMIndexFiles
{
    public : 

    ArDMIndexFiles(const std::string & dirFiles);
    ~ArDMIndexFiles();

    std::vector<std::string> readIndexFileList(void);
    std::string getRunID(char *idRun);

    private :
    std::vector<std::string> m_files; 
    std::string  m_path;
};


class ArDMData
{
  public:
  // dirFiles : Directory where are raw and index files
  // runNumber : the id run 
  // ex:   ArDMData *aFile = ArDMData("/usr/data/","3297") 
  ArDMData(const std::string & dirFiles,const std::string & runNumber);
  ~ArDMData();
           long * readSubEvent(tIndexItem *evt) {return readBlock(evt); };
           long * readSubEvent(int idEvent,bool last);
           long * readSubEvent(int idEvent,tIndexItem *evt,bool last);
  unsigned long   getRunTimeStamp(void);
  unsigned long   getTimeStamp(void);
           float  getRealTTT(unsigned long ttt);
  //unsigned long   getTTT(void);
  uint32_t   getTTT(void);
  unsigned int    getCrate(void);
  unsigned int    getBoard(void);
  unsigned int    getEventID(void);
  unsigned int    getNbChannels(void);
  unsigned int    getNbChannels(int crateid,int boardid);
           int    getLightData(int channelid,unsigned int idData);
           int    getData(unsigned int idData);
  unsigned int    getNbData(void);
  unsigned int    getFlags(void);
  unsigned int    getChannel(void);
  unsigned int    getLostTrigger(void);
           long * getPtData(void);
  unsigned long   getIdRun(void);
  unsigned int    getCrateMask(void);
  unsigned int    getBoardMask(int crate);
  unsigned int    getBufferSize(void);
  unsigned long   getKey(void);

  unsigned long   getChannelMask(int crateid,int boardid);
  unsigned long   getLowThreshold(int crateid,int boardid,int channelid)
    {return getThreshold("LTHRES",crateid,boardid,channelid);};
  unsigned long   getHighThreshold(int crateid,int boardid,int channelid)
    {return getThreshold("HTHRES",crateid,boardid,channelid);};
  unsigned long   getSamplingRate(void) ;
  unsigned long   getDACA(int crateid,int boardid) {return getDAC('A',crateid,boardid);};
  unsigned long   getDACB(int crateid,int boardid) {return getDAC('B',crateid,boardid);};
  unsigned long   getPostTrigger(int crateid,int boardid);
  unsigned long   getTotalSize(void);
  unsigned long   getNbEvent(void);
  std::string     getCurrentFile(int crateId);
  unsigned long   getCurrentIdEvent(void);
  unsigned int    getNbCrate(void);
           int    readLastPos(tIndexItem *evt);
           int    readPos(tIndexItem *evt,int idSubEvent);
           bool   isDataReadable();
           int    checkIndexFile();
           int    getSubEvent(int eventID,int board,tIndexItem *evt);
  int    getEvent_checkTTT(int eventID,tIndexItem evt[]); //khoi's version 2 //check the TTT !
  int    getEvent_checkEventNumber(int eventID,tIndexItem evt[]); //khoi's version 1 //check the event number
  int    getEvent_checkEventNumber_thierry(int eventID,tIndexItem evt[]);//Thierry's
 
  int getEvent(int eventID,tIndexItem evt[]);

  private:
  unsigned long   getThreshold(const char* kind,int crateid,int boardid,int channelid);
  unsigned long   getDAC(char kind,int crateid,int boardid);
           bool   readParams(void);
           long * readBlock(tIndexItem *evt);
      tcheckDAQ   m_checkDAQ[C_NBMAX_PMT_BOARD];

  std::ifstream indexFile;
  std::ifstream  datafile ;
  std::string m_name, m_dir,m_runNumber ;
  int version;
  char m_currentFileName[1024];
  char m_currentParametersFileName[1024];
  
  //test // khoi
  unsigned int m_lastPositionInIndexFile;
  uint32_t     m_lastTTT;
  int          m_lastEventFlag;
  //end test

  public : 
  long          *   buffData;

  private : 
  unsigned long     m_key;
  unsigned int      m_sizeTTEvent;
  unsigned long     m_timeStamp;
  unsigned int      m_crate;
  unsigned int      m_board;
  unsigned int      m_flags;
  unsigned int      m_channel;
  unsigned int      m_lostTrigger;
  unsigned int      m_sizeData;
  //unsigned long     m_TTT;
  uint32_t     m_TTT;
  unsigned long     m_eventID;
           long *   m_ptData;
  unsigned long     m_lthres[C_MAX_CRATE][C_NBMAX_BOARD][C_NBMAX_CHANNEL_PER_BOARD];
  unsigned long     m_hthres[C_MAX_CRATE][C_NBMAX_BOARD][C_NBMAX_CHANNEL_PER_BOARD];
  unsigned long     m_dac[2][C_MAX_CRATE][C_NBMAX_BOARD]; 
  unsigned long     m_postTrigger[C_MAX_CRATE][C_NBMAX_BOARD]; 
  unsigned long     m_channelMask[C_MAX_CRATE][C_NBMAX_BOARD]; 
  unsigned long     m_idrun;
  unsigned long     m_samplingRate;
  int               m_maskCrate;
  int               m_nbCrate;
  int               m_maskBoard[C_MAX_CRATE];
  int               m_bufferOrg;
  unsigned long     m_nbEvent;
  unsigned long     m_nbSubEvent;
  std::string       m_currentFile[C_MAX_CRATE];
  unsigned long     m_idEvent;
           bool     m_indexFilesConsistent;

};


class ArDMRawDataFile
{
  public:

  ArDMRawDataFile (const std::string & dirFiles,const std::string & begNumber,const std::string & endNumber,char type);
  void BuildIndexFile(char * nameFile);

  ArDMData * aFile;
  int m_begNumber,m_endNumber;
  std::string m_dir;
  char m_type;
  std::vector<tIndexItem > indexfile;
};


#endif

