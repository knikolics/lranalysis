void CreateConfigFile(Int_t runnumber, Int_t from=0, Int_t to=-1)
{
  char cfgname[20];
  sprintf(cfgname,"Run%04d-File.cfg",runnumber);
  char dir[20];
  sprintf(dir,"Run%04d",runnumber);

  FILE *fp = fopen(cfgname,"w+");

  fprintf(fp,"DEBUGLEVEL 0							\n");
  fprintf(fp,"BIT 12								\n");
  fprintf(fp,"SAMPLE_SIZE 4.		       					\n");
  fprintf(fp,"EVENTS %d %d							\n",from,to);
  fprintf(fp,"#EVENTS_MASK 1 0 92						\n");
  fprintf(fp,"#EVENTS_MASK 3 0 171 1 50 6 99					\n");
  fprintf(fp,"#EVENTS_MASK_FILE %s/mask.dat					\n",dir);
  fprintf(fp,"									\n");
  fprintf(fp,"#CALIBRATION_FILE calibration14.dat				\n");
  
  //modify this to 24 pmts !!
  //for the time being, we don't need the calibration yet, so just leave it there !
  fprintf(fp,"CALIBRATION 16 1. 1. 1. 1. 1. 1. 1. 1. 1. 1. 1. 1. 1. 1. 1. 1.	\n");
  fprintf(fp,"REFERENCE_CONVERSION_FACTOR 1.0					\n");
  fprintf(fp,"									\n");

  //modify this to 24 pmts !!
  fprintf(fp,"#DELAY 14 0 0 0 0 0 0 0 0 0 0 0 0					\n");
  fprintf(fp,"									\n");
  fprintf(fp,"APPLY_DELAY_CORRECTION_FROM_DAQ_INFO 1				\n");
  fprintf(fp,"EVALUATE_CHANNELS_FROM_VIRTUAL_CHANNEL 1				\n");
  fprintf(fp,"									\n");
  fprintf(fp,"#SET_FAKE_SIGNAL_TIME 1000.					\n");
  fprintf(fp,"SINGLET_INTEGRATION_TIME 50.					\n");
  fprintf(fp,"PEDESTAL_PEAK_FINDING_TIME_MARGIN 10.				\n");
  fprintf(fp,"PEAK_FINDING_TIME_MARGIN 20.					\n");
  fprintf(fp,"PEDESTAL_PEAK_FINDING_DISENTANGLE_TIME 40. 			\n");
  fprintf(fp,"PEAK_FINDING_DISENTANGLE_TIME 3.					\n");
  fprintf(fp,"PEDESTAL_PEAK_FINDING_SIGMA_THRESHOLD 4.				\n");
  fprintf(fp,"PEAK_FINDING_SIGMA_THRESHOLD 10.					\n");
  fprintf(fp,"									\n");
  fprintf(fp,"#PEDESTAL_TIME_RANGE 30 600					\n");
  fprintf(fp,"#PEDESTAL_TIME_RANGE 7000 10000					\n");
  fprintf(fp,"PEDESTAL_TIME_RANGE 30 200					\n");
  fprintf(fp,"									\n");
  fprintf(fp,"#PEAK_SIGNAL_TIME_RANGE 600 2000					\n");

  //fprintf(fp,"PEAK_SIGNAL_TIME_RANGE 30 2000					\n");

  fprintf(fp,"PEAK_SIGNAL_TIME_RANGE 30 4000					\n");

  fprintf(fp,"									\n");
  //fprintf(fp,"VIRTUAL_CHANNEL_MASK 16 1 1 1 1 1 1 1 1 1 1 1 1 1 1 0 0		\n");
  fprintf(fp,"VIRTUAL_CHANNEL_MASK 28 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 0 0 0 0		\n");
  fprintf(fp,"									\n");
  fprintf(fp,"MAXIMUM_INTEGRATION_TIME 4500.					\n");
  fprintf(fp,"									\n");

  fclose(fp);
}
