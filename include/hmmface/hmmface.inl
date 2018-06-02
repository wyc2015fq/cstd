typedef struct hmmface_t {
  //超态及其每个状态的混高斯
  int m_stnum[32];// = {5, 3, 6, 6, 6, 3};
  int m_mixnum[128];
  BOOL m_suppress_intensity;  //是否抑制直流分量
  CvSize m_delta;//用于放进观测向量的水平和垂直方向上的最小DCT系数
  CvSize m_obsSize;
  CvSize m_dctSize;//用DCT（离散余弦变换）系数计算的图像块的大小
  int m_vectSize; //用来创建HMM的观察向量的大小
} hmmface_t;
BOOL hmmface_save(hmmface_t* s, const CvEHMM* m_hmm, const char* filename)
{
  int i, j, k, m;
  FILE* file = NULL;
  const CvEHMM* hmm = m_hmm;
  float* prob = NULL;
  if (!m_hmm) {
    return FALSE;
  }
  file = fopen(filename, "wt");
  if (!file) {
    return FALSE;
  }
  // write topology
  fprintf(file, "%s %d\n", "<NumSuperStates>", m_hmm->num_states);
  fprintf(file, "%s ", "<NumStates>");
  for (i = 0; i < m_hmm->num_states; i++) {
    fprintf(file, "%d ", m_hmm->u.ehmm[i].num_states);
  }
  fprintf(file, "\n");
  fprintf(file, "%s ", "<NumMixtures>");
  for (i = 0; i < m_hmm->num_states; i++) {
    CvEHMM* ehmm = &(m_hmm->u.ehmm[i]);
    for (j = 0; j < ehmm->num_states; j++) {
      fprintf(file, "%d ", ehmm->u.state[j].num_mix);
    }
  }
  fprintf(file, "\n");
  fprintf(file, "%s %d\n", "<VecSize>", s->m_vectSize);
  //consequently write all hmms
  for (i = 0; i < m_hmm->num_states + 1; i++) {
    if (hmm->level == 0) {
      fprintf(file, "%s\n", "<BeginEmbeddedHMM>");
    }
    else {
      fprintf(file, "%s\n", "<BeginExternalHMM>");
    }
    fprintf(file, "%s %d\n", "<NumStates>", hmm->num_states);
    if (hmm->level == 0) {
      for (j = 0; j < hmm->num_states; j++) {
        CvEHMMState* state = &(hmm->u.state[j]);
        float* mu = state->mu;
        float* inv_var = state->inv_var;
        fprintf(file, "%s %d\n", "<State>", j);
        fprintf(file, "%s %d\n", "<NumMixes>", state->num_mix);
        for (m = 0; m < state->num_mix; m++) {
          fprintf(file, "%s %d %s %f\n", "<Mixture>", m, "<Weight>", state->weight[m]);
          fprintf(file, "%s\n", "<Mean>");
          for (k = 0; k < s->m_vectSize; k++) {
            fprintf(file, "%f ", mu[0]);
            mu++;
          }
          fprintf(file, "\n");
          fprintf(file, "%s\n", "<Inverted_Deviation>");
          for (k = 0; k < s->m_vectSize; k++) {
            fprintf(file, "%f ", inv_var[0]);
            inv_var++;
          }
          fprintf(file, "\n");
          fprintf(file, "%s %f\n", "<LogVarVal>", state->log_var_val[m]);
        }
      }
    }
    //write the transition probability matrix
    fprintf(file, "%s\n", "<TransP>");
    prob = hmm->transP;
    for (j = 0; j < hmm->num_states; j++) {
      for (k = 0; k < hmm->num_states; k++) {
        fprintf(file, "%f ", *prob);
        prob++;
      }
      fprintf(file, "\n");
    }
    if (hmm->level == 0) {
      fprintf(file, "%s\n", "<EndEmbeddedHMM>");
    }
    else {
      fprintf(file, "%s\n", "<EndExternalHMM>");
    }
    hmm = &(m_hmm->u.ehmm[i]);
  }
  fclose(file);
  return TRUE;
}
#if 0
BOOL hmmface_load(CvEHMM* m_hmm, const char* filename)
{
  FILE* file;
  CvEHMM* hmm = m_hmm;
  int num_states[128];
  int num_mix[128];
  char temp_char[128];
  if (m_hmm) {
    cvRelease2DHMM(&m_hmm);
  }
  file = fopen(filename, "rt");
  if (!file) {
    return FALSE;
  }
  // read topology
  fscanf(file, "%s %d\n", temp_char, num_states);
  fscanf(file, "%s ", temp_char);
  for (i = 0; i < num_states[0]; i++) {
    fscanf(file, "%d ", num_states + i + 1);
  }
  fscanf(file, "\n");
  //compute total number of internal states
  int total_states = 0;
  for (i = 0; i < num_states[0]; i++) {
    total_states += num_states[i + 1];
  }
  //read number of mixtures
  fscanf(file, "%s ", temp_char);
  for (i = 0; i < total_states; i++) {
    fscanf(file, "%d ", &num_mix[i]);
  }
  fscanf(file, "\n");
  fscanf(file, "%s %d\n", temp_char, &m_vectSize);
  m_hmm = cvCreate2DHMM(num_states, num_mix, m_vectSize);
  //create HMM with known parameters
  //!!!    cvCreate2DHMM( &m_hmm, num_states, num_mix, m_vectSize);
  //consequently read all hmms
  for (i = 0; i < num_states[0] + 1; i++) {
    fscanf(file, "%s\n", temp_char);
    int temp_int;
    fscanf(file, "%s %d\n", temp_char , &temp_int);
    assert(temp_int == num_states[i]);
    if (i != 0) {
      for (j = 0; j < num_states[i]; j++) {
        CvEHMMState* state = &(hmm->u.state[j]);
        fscanf(file, "%s %d\n", temp_char, &temp_int);
        assert(temp_int == j);
        fscanf(file, "%s %d\n", temp_char, &temp_int);
        assert(temp_int == state->num_mix);
        float* mu = state->mu;
        float* inv_var = state->inv_var;
        for (m = 0; m < state->num_mix; m++) {
          int temp_int;
          fscanf(file, "%s %d %s %f\n", temp_char, &temp_int, temp_char, &(state->weight[m]));
          assert(temp_int == m);
          fscanf(file, "%s\n", temp_char);
          for (k = 0; k < m_vectSize; k++) {
            fscanf(file, "%f ", mu);
            mu++;
          }
          fscanf(file, "\n");
          fscanf(file, "%s\n", temp_char);
          for (k = 0; k < m_vectSize; k++) {
            fscanf(file, "%f ", inv_var);
            inv_var++;
          }
          fscanf(file, "\n");
          fscanf(file, "%s %f\n", temp_char, &(state->log_var_val[m]));
        }
      }
    }
    //read the transition probability matrix
    fscanf(file, "%s\n", temp_char);
    float* prob = hmm->transP;
    for (j = 0; j < hmm->num_states; j++) {
      for (k = 0; k < hmm->num_states; k++) {
        fscanf(file, "%f ", prob);
        prob++;
      }
      fscanf(file, "\n");
    }
    fscanf(file, "%s\n", temp_char);
    hmm = &(m_hmm->u.ehmm[i]);
  }
  fclose(file);
  return true;
}
#endif

