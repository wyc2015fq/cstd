/*********************************************************************
/*
/* Core of the Layer II decoder. Default layer is Layer II.
/*
/*********************************************************************/
/* Global variable definitions for "musicout.c" */
char* programName;
static void usage() /* print syntax & exit */
{
  fprintf(stderr, "usage: %s queries for all arguments, or\n", programName);
  fprintf(stderr, " %s [-A][-s sb] inputBS [outPCM]\n", programName);
  fprintf(stderr, "where\n");
  fprintf(stderr, " -A write an wav output PCM sound file\n");
  fprintf(stderr, " -s sb resynth only up to this sb (debugging only)\n");
  fprintf(stderr, " inputBS input bit stream of encoded audio\n");
  fprintf(stderr, " outPCM output PCM sound file (dflt inName+%s)\n", DFLT_OPEXT);
  exit(1);
}
/* Implementations */
void main(int argc, char** argv)
{
  /*typedef short PCM[2][3][SBLIMIT];*/
  typedef short PCM[2][SSLIMIT][SBLIMIT];
  PCM FAR* pcm_sample;
  typedef unsigned int SAM[2][3][SBLIMIT];
  SAM FAR* sample;
  typedef double FRA[2][3][SBLIMIT];
  FRA FAR* fraction;
  typedef double VE[2][HAN_SIZE];
  VE FAR* w;
  Bit_stream_struc bs;
  frame_params fr_ps;
  layer info;
  FILE* musicout;
  unsigned long sample_frames;
  int i, j, k, x, stereo, done = FALSE, clip, sync;
  int error_protection, crc_error_count, total_error_count;
  unsigned int old_crc, new_crc;
  unsigned int bit_alloc[2][SBLIMIT], scfsi[2][SBLIMIT],
           scale_index[2][3][SBLIMIT];
  unsigned long bitsPerSlot, samplesPerFrame, frameNum = 0;
  unsigned long frameBits, gotBits = 0;
  IFF_AIFF pcm_aiff_data;
  char encoded_file_name[MAX_NAME_SIZE];
  char decoded_file_name[MAX_NAME_SIZE];
  char default_file_name[MAX_NAME_SIZE];
  char t[50];
  int need_aiff;
  int need_esps; /* MI */
  int topSb = 0;
  III_scalefac_t III_scalefac;
  III_side_info_t III_side_info;
#ifdef MACINTOSH
  console_options.nrows = MAC_WINDOW_SIZE;
  argc = ccommand(&argv);
#endif
  /* Most large variables are declared dynamically to ensure compatibility with smaller machines */
  pcm_sample = (PCM FAR*) mem_alloc((long) sizeof(PCM), "PCM Samp");
  sample = (SAM FAR*) mem_alloc((long) sizeof(SAM), "Sample");
  fraction = (FRA FAR*) mem_alloc((long) sizeof(FRA), "fraction");
  w = (VE FAR*) mem_alloc((long) sizeof(VE), "w");
  fr_ps.header = &info;
  fr_ps.tab_num = -1; /* no table loaded */
  fr_ps.alloc = NULL;
  for (i = 0; i < HAN_SIZE; i++) {
    for (j = 0; j < 2; j++) {
      (*w)[j][i] = 0.0;
    }
  }
  programName = argv[0];
  if (argc == 1) { /* no command line args -> interact */
    do {
      printf("Enter encoded file name <required>: ");
      gets(encoded_file_name);
      if (encoded_file_name[0] == NULL_CHAR) {
        printf("Encoded file name is required. \n");
      }
    }
    while (encoded_file_name[0] == NULL_CHAR);
    printf(">>> Encoded file name is: %s \n", encoded_file_name);
    x = 0;
    while (x <= MAX_NAME_SIZE) {
      default_file_name[x] = NULL_CHAR;
      ++x;
    }
    x = 0;
    while (x <= 8) {
      default_file_name[x] = encoded_file_name[x];
      if (encoded_file_name[++x] == '.') {
        x = 9;
      }
    }
    strcat(default_file_name, DFLT_OPEXT);
    printf("Enter MPEG decoded output file name <%s>: ",
        default_file_name); /* 92-08-19 shn */
    gets(decoded_file_name);
    if (decoded_file_name[0] == NULL_CHAR) {
      /* replace old extension with new one, 92-08-19 shn */
      strcpy(decoded_file_name, default_file_name);
    }
    printf(">>> MPEG decoded file name is: %s \n", decoded_file_name);
    printf("Do you wish to write an WAV compatible sound file ? (y/<n>) : ");
    // printf("Do you wish to write an AIFF compatible sound file ? (y/<n>) : ");
    gets(t);
    if (*t == 'y' || *t == 'Y') {
      need_aiff = TRUE;
    }
    else {
      need_aiff = FALSE;
    }
    if (need_aiff) {
      printf(">>> An WAV compatible sound file will be written\n");
    }
    // printf(">>> An AIFF compatible sound file will be written\n");
    else {
      printf(">>> A non-headered PCM sound file will be written\n");
    }
    printf(
        "Do you wish to exit (last chance before decoding) ? (y/<n>) : ");
    gets(t);
    if (*t == 'y' || *t == 'Y') {
      exit(0);
    }
  }
  else { /* interpret CL Args */
    int i = 0, err = 0;
    need_aiff = FALSE;
    need_esps = FALSE; /* MI */
    encoded_file_name[0] = '\0';
    decoded_file_name[0] = '\0';
    while (++i < argc && err == 0) {
      char c, *token, *arg, *nextArg;
      int argUsed;
      token = argv[i];
      if (*token++ == '-') {
        if (i + 1 < argc) {
          nextArg = argv[i + 1];
        }
        else {
          nextArg = "";
        }
        argUsed = 0;
        while (c = *token++) {
          if (*token /* NumericQ(token) */) {
            arg = token;
          }
          else {
            arg = nextArg;
          }
          switch (c) {
          case 's':
            topSb = atoi(arg);
            argUsed = 1;
            if (topSb < 1 || topSb > SBLIMIT) {
              fprintf(stderr, "%s: -s band %s not %d..%d\n",
                  programName, arg, 1, SBLIMIT);
              err = 1;
            }
            break;
          case 'A':
            need_aiff = TRUE;
            break;
          case 'E':
            need_esps = TRUE;
            break; /* MI */
          default:
            fprintf(stderr, "%s: unrecognized option %c\n", programName, c);
            err = 1;
            break;
          }
          if (argUsed) {
            if (arg == token) {
              token = ""; /* no more from token */
            }
            else {
              ++i; /* skip arg we used */
            }
            arg = "";
            argUsed = 0;
          }
        }
      }
      else {
        if (encoded_file_name[0] == '\0') {
          strcpy(encoded_file_name, argv[i]);
        }
        else if (decoded_file_name[0] == '\0') {
          strcpy(decoded_file_name, argv[i]);
        }
        else {
          fprintf(stderr,
              "%s: excess arg %s\n", programName, argv[i]);
          err = 1;
        }
      }
    }
    if (err || encoded_file_name[0] == '\0') {
      usage(); /* never returns */
    }
    if (decoded_file_name[0] == '\0') {
      strcpy(decoded_file_name, encoded_file_name);
      strcat(decoded_file_name, DFLT_OPEXT);
    }
  }
  /* report results of dialog / command line */
  printf("Input file = '%s' output file = '%s'\n", encoded_file_name, decoded_file_name);
  if (need_aiff) {
    printf("Output file written in wav format\n");
  }
  if (need_esps) {
    printf("Output file written in ESPS format\n"); /* MI */
  }
  if ((musicout = fopen(decoded_file_name, "w+b")) == NULL) {//decoded_file_name: 2.dec
    printf("Could not create \"%s\".\n", decoded_file_name);
    exit(1);
  }
  open_bit_stream_r(&bs, encoded_file_name, BUFFER_SIZE);//encoded_file_name: 2.mp3
  if (need_aiff) {
    if (aiff_seek_to_sound_data(musicout) == -1) {
      printf("Could not seek to PCM sound data in \"%s\".\n", decoded_file_name);
      exit(1);
    }
  }
  sample_frames = 0;
  while (!end_bs(&bs)) {
    //尝试帧同步
    sync = seek_sync(&bs, SYNC_WORD, SYNC_WORD_LNGTH);
    frameBits = sstell(&bs) - gotBits;
    if (frameNum > 0) { /* don't want to print on 1st loop; no lay */
      if (frameBits % bitsPerSlot) {
        fprintf(stderr, "Got %ld bits = %ld slots plus %ld\n", frameBits, frameBits / bitsPerSlot, frameBits % bitsPerSlot);
      }
    }
    gotBits += frameBits;
    if (!sync) {
      printf("Frame cannot be located\n");
      printf("Input stream may be empty\n");
      done = TRUE;
      /* finally write out the buffer */
      if (info.lay != 1) {
        out_fifo(*pcm_sample, 3, &fr_ps, done, musicout, &sample_frames);
      }
      else {
        out_fifo(*pcm_sample, 1, &fr_ps, done, musicout, &sample_frames);
      }
      break;
    }
    //解码帧头
    decode_info(&bs, &fr_ps);
    //将fr_ps.header中的信息解读到fr_ps的相关域中
    hdr_to_frps(&fr_ps);
    stereo = fr_ps.stereo;
    error_protection = info.error_protection;
    crc_error_count = 0;
    total_error_count = 0;
    //输出相关信息
    if (frameNum == 0) {
      WriteHdr(&fr_ps, stdout); /* printout layer/mode */
    }
#ifdef ESPS
    if (frameNum == 0 && need_esps) {
      esps_write_header(musicout, (long) sample_frames, (double)s_freq[info.sampling_frequency] * 1000, (int) stereo, decoded_file_name);
    } /* MI */
#endif
    fprintf(stderr, "{%4lu}", frameNum++);
    fflush(stderr);
    if (error_protection) {
      buffer_CRC(&bs, &old_crc);
    }
    switch (info.lay) {
    case 3: {
      int nSlots;
      int gr, ch, ss, sb, main_data_end, flush_main ;
      int bytes_to_discard ;
      static int frame_start = 0;
      bitsPerSlot = 8;
      samplesPerFrame = 1152;
      //取Side信息
      III_get_side_info(&bs, &III_side_info, &fr_ps);
      nSlots = main_data_slots(fr_ps);
      //读主数据(Audio Data)
      for (; nSlots > 0; nSlots--) { /* read main data. */
        hputbuf((unsigned int) getbits(&bs, 8), 8);
      }
      main_data_end = hsstell() / 8; /*of privious frame*/
      if (flush_main = (hsstell() % bitsPerSlot)) {
        hgetbits((int)(bitsPerSlot - flush_main));
        main_data_end ++;
      }
      bytes_to_discard = frame_start - main_data_end - III_side_info.main_data_begin ;
      if (main_data_end > 4096) {
        frame_start -= 4096;
        rewindNbytes(4096);
      }
      frame_start += main_data_slots(fr_ps);
      if (bytes_to_discard < 0) {
        printf("Not enough main data to decode frame %d. Frame discarded.\n",
            frameNum - 1);
        break;
      }
      for (; bytes_to_discard > 0; bytes_to_discard--) {
        hgetbits(8);
      }
      clip = 0;
      for (gr = 0; gr < 2; gr++) {
        double lr[2][SBLIMIT][SSLIMIT], ro[2][SBLIMIT][SSLIMIT];
        //主解码
        for (ch = 0; ch < stereo; ch++) {
          long int is[SBLIMIT][SSLIMIT]; /* Quantized samples. 保存量化数据*/
          int part2_start;
          part2_start = hsstell();
          //获取比例因子
          III_get_scale_factors(III_scalefac, &III_side_info, gr, ch, &fr_ps);
          //Huffman解码
          III_hufman_decode(is, &III_side_info, ch, gr, part2_start, &fr_ps);
          //反量化采样
          III_dequantize_sample(is, ro[ch], &III_scalefac, &(III_side_info.ch[ch].gr[gr]), ch, &fr_ps);
        }
        //立体声处理
        III_stereo(ro, lr, III_scalefac, &(III_side_info.ch[0].gr[gr]), &fr_ps);
        for (ch = 0; ch < stereo; ch++) {
          double re[SBLIMIT][SSLIMIT];
          double hybridIn[SBLIMIT][SSLIMIT];/* Hybrid filter input */
          double hybridOut[SBLIMIT][SSLIMIT];/* Hybrid filter out */
          double polyPhaseIn[SBLIMIT]; /* PolyPhase Input. */
          III_reorder(lr[ch], re, &(III_side_info.ch[ch].gr[gr]), &fr_ps);
          //抗锯齿处理
          III_antialias(re, hybridIn, & (III_side_info.ch[ch].gr[gr]), &fr_ps); /* Antialias butterflies. */
          for (sb = 0; sb < SBLIMIT; sb++) { /* Hybrid synthesis. */
            III_hybrid(hybridIn[sb], hybridOut[sb], sb, ch, &(III_side_info.ch[ch].gr[gr]), &fr_ps);
          }
          for (ss = 0; ss < 18; ss++) {/*Frequency inversion for polyphase.多相频率倒置*/
            for (sb = 0; sb < SBLIMIT; sb++) {
              if ((ss % 2) && (sb % 2)) {
                hybridOut[sb][ss] = -hybridOut[sb][ss];
              }
            }
          }
          for (ss = 0; ss < 18; ss++) { /* Polyphase synthesis 多相合成*/
            for (sb = 0; sb < SBLIMIT; sb++) {
              polyPhaseIn[sb] = hybridOut[sb][ss];
            }
            clip += SubBandSynthesis(polyPhaseIn, ch, &((*pcm_sample)[ch][ss][0]));
          }
        }
        /* Output PCM sample points for one granule.子带合成 */
        out_fifo(*pcm_sample, 18, &fr_ps, done, musicout, &sample_frames);
      }
      if (clip > 0) {
        printf("%d samples clipped.\n", clip);
      }
      break;
    }
    /*case 1: {
    bitsPerSlot = 32; samplesPerFrame = 384;
    I_decode_bitalloc(&bs,bit_alloc,&fr_ps);
    I_decode_scale(&bs, bit_alloc, scale_index, &fr_ps);
    if (error_protection) {
    I_CRC_calc(&fr_ps, bit_alloc, &new_crc);
    if (new_crc != old_crc) {
    crc_error_count++;
    total_error_count++;
    recover_CRC_error(*pcm_sample, crc_error_count,
    &fr_ps, musicout, &sample_frames);
    break;
    }
    else crc_error_count = 0;
    }
    clip = 0;
    for (i=0;i<SCALE_BLOCK;i++) {
    I_buffer_sample(&bs,(*sample),bit_alloc,&fr_ps);
    I_dequantize_sample(*sample,*fraction,bit_alloc,&fr_ps);
    I_denormalize_sample((*fraction),scale_index,&fr_ps);
    if(topSb>0) // clear channels to 0
    for(j=topSb; j<fr_ps.sblimit; ++j)
    for(k=0; k<stereo; ++k)
    (*fraction)[k][0][j] = 0;
    for (j=0;j<stereo;j++) {
    clip += SubBandSynthesis (&((*fraction)[j][0][0]), j,
    &((*pcm_sample)[j][0][0]));
    }
    out_fifo(*pcm_sample, 1, &fr_ps, done,
    musicout, &sample_frames);
    }
    if(clip > 0) printf("%d output samples clipped\n", clip);
    break;
    }
    case 2: {
    bitsPerSlot = 8; samplesPerFrame = 1152;
    II_decode_bitalloc(&bs, bit_alloc, &fr_ps);
    II_decode_scale(&bs, scfsi, bit_alloc, scale_index, &fr_ps);
    if (error_protection) {
    II_CRC_calc(&fr_ps, bit_alloc, scfsi, &new_crc);
    if (new_crc != old_crc) {
    crc_error_count++;
    total_error_count++;
    recover_CRC_error(*pcm_sample, crc_error_count,
    &fr_ps, musicout, &sample_frames);
    break;
    }
    else crc_error_count = 0;
    }
    clip = 0;
    for (i=0;i<SCALE_BLOCK;i++) {
    II_buffer_sample(&bs,(*sample),bit_alloc,&fr_ps);
    II_dequantize_sample((*sample),bit_alloc,(*fraction),&fr_ps);
    II_denormalize_sample((*fraction),scale_index,&fr_ps,i>>2);
    if(topSb>0) // debug : clear channels to 0
    for(j=topSb; j<fr_ps.sblimit; ++j)
    for(k=0; k<stereo; ++k)
    (*fraction)[k][0][j] =
    (*fraction)[k][1][j] =
    (*fraction)[k][2][j] = 0;
    for (j=0;j<3;j++) for (k=0;k<stereo;k++) {
    clip += SubBandSynthesis (&((*fraction)[k][j][0]), k,
    &((*pcm_sample)[k][j][0]));
    }
    out_fifo(*pcm_sample, 3, &fr_ps, done, musicout,
    &sample_frames);
    }
    if(clip > 0) printf("%d samples clipped\n", clip);
    break;
    }*/
    }
  }
  if (need_aiff) {
    //给音频数据加个wav文件头
    WAVE_FORMAT waveformat;
    FMT_BLOCK fmtblk;
    RIFF_HEADER riffheder;
    DATA_BLOCK datablock;
    int filelen;
    memset(&waveformat, 0, sizeof(waveformat));
    memset(&fmtblk, 0, sizeof(FMT_BLOCK));
    memset(&riffheder, 0, sizeof(RIFF_HEADER));
    memset(&datablock, 0, sizeof(DATA_BLOCK));
    fseek(musicout, 0, SEEK_END);
    filelen = ftell(musicout);
    fseek(musicout, 0, SEEK_SET);
    waveformat.wFormatTag = 0x0001;
    waveformat.wChannels = stereo;
    waveformat.wBitsPerSample = 0x0010;
    waveformat.dwSamplesPerSec = s_freq[info.sampling_frequency] * 1000;
    waveformat.wBlockAlign = waveformat.wChannels * (waveformat.wBitsPerSample / 8); //0x0004;
    waveformat.dwAvgBytesPerSec = waveformat.dwSamplesPerSec * waveformat.wBlockAlign;
    fmtblk.szFmtID[0] = 'f';
    fmtblk.szFmtID[1] = 'm';
    fmtblk.szFmtID[2] = 't';
    fmtblk.szFmtID[3] = ' ';
    fmtblk.dwFmtSize = 16;
    memcpy((void*)&fmtblk.wavFormat, (void*)&waveformat, sizeof(WAVE_FORMAT));
    riffheder.szRiffID[0] = 'R';
    riffheder.szRiffID[1] = 'I';
    riffheder.szRiffID[2] = 'F';
    riffheder.szRiffID[3] = 'F';
    riffheder.szRiffFormat[0] = 'W';
    riffheder.szRiffFormat[1] = 'A';
    riffheder.szRiffFormat[2] = 'V';
    riffheder.szRiffFormat[3] = 'E';
    riffheder.dwRiffSize = filelen + sizeof(RIFF_HEADER) + sizeof(FMT_BLOCK) + sizeof(DATA_BLOCK) - 8;
    datablock.szDataID[0] = 'd';
    datablock.szDataID[1] = 'a';
    datablock.szDataID[2] = 't';
    datablock.szDataID[3] = 'a';
    datablock.dwDataSize = filelen;
    if (fwrite(&riffheder, sizeof(RIFF_HEADER), 1, musicout) != 1) {
      exit(2);
    }
    if (fwrite(&fmtblk, sizeof(FMT_BLOCK), 1, musicout) != 1) {
      exit(2);
    }
    if (fwrite(&datablock, sizeof(DATA_BLOCK), 1, musicout) != 1) {
      exit(2);
    }
    /*给音频数据加AIFF文件头
    pcm_aiff_data.numChannels = stereo;
    pcm_aiff_data.numSampleFrames = sample_frames;
    pcm_aiff_data.sampleSize = 16;
    pcm_aiff_data.sampleRate = s_freq[info.sampling_frequency]*1000;
    #ifdef IFF_LONG
    pcm_aiff_data.sampleType = IFF_ID_SSND;
    #else
    strncpy(&pcm_aiff_data.sampleType,IFF_ID_SSND,4);
    #endif
    pcm_aiff_data.blkAlgn.offset = 0;
    pcm_aiff_data.blkAlgn.blockSize = 0;
    if (aiff_write_headers(musicout, &pcm_aiff_data) == -1) {
    printf("Could not write AIFF headers to \"%s\"\n",
    decoded_file_name);
    exit(2);
    }*/
  }
  printf("Avg slots/frame = %.3f; b/smp = %.2f; br = %.3f kbps\n",
      (FLOAT) gotBits / (frameNum * bitsPerSlot),
      (FLOAT) gotBits / (frameNum * samplesPerFrame),
      (FLOAT) gotBits / (frameNum * samplesPerFrame) *
      s_freq[info.sampling_frequency]);
  close_bit_stream_r(&bs);
  fclose(musicout);
  /* for the correct AIFF header information */
  /* on the Macintosh */
  /* the file type and the file creator for */
  /* Macintosh compatible Digidesign is set */
#ifdef MACINTOSH
  if (need_aiff) set_mac_file_attr(decoded_file_name, VOL_REF_NUM,
        CREATR_DEC_AIFF, FILTYP_DEC_AIFF);
  else set_mac_file_attr(decoded_file_name, VOL_REF_NUM,
        CREATR_DEC_BNRY, FILTYP_DEC_BNRY);
#endif
  printf("Decoding of \"%s\" is finished\n", encoded_file_name);
  printf("The decoded PCM output file name is \"%s\"\n", decoded_file_name);
  if (need_aiff)
    printf("\"%s\" has been written with WAV header information\n",
        decoded_file_name);
  //printf("\"%s\" has been written with AIFF header information\n",
  // decoded_file_name);
  exit(0);
}

