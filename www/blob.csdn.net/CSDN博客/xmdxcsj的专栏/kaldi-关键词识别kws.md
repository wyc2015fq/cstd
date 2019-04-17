# kaldi-关键词识别kws - xmdxcsj的专栏 - CSDN博客





2016年03月02日 21:18:00[xmucas](https://me.csdn.net/xmdxcsj)阅读数：5755








将kaldi中的kws部分的脚本跑通，并且把所有的二进制命令使用一个脚本串联起来。



```
#######################################
##files dir
#######################################
dataset_id=news
keyword_id=news
#input pcm data
pcm_dir=pcm/$dataset_id
wav_scp=$pcm_dir/wav.scp
utt2spk=$pcm_dir/utt2spk
spk2utt=$pcm_dir/spk2utt
utter_id=$pcm_dir/utter_id
ref_ecf=$pcm_dir/ecf.xml
ref_rttm=$pcm_dir/rttm
#feature conf
featconf_dir=conf/feature
fbank_config=$featconf_dir/fbank.conf
pitch_config=$featconf_dir/pitch.conf
pitch_postprocess_config=$featconf_dir/pitch_post.conf
#feature
feats_dir=0.feats/$dataset_id
mkdir -p $feats_dir
fbank_ark=$feats_dir/feats.ark
feats_log=$feats_dir/feats.log
feats_scp=$feats_dir/feats.scp
cmvn_ark=$feats_dir/cmvn.ark
cmvn_log=$feats_dir/cmvn.log
cmvn_scp=$feats_dir/cmvn.scp

#input keyword data
keywords_dir=1.keywords/$keyword_id
keywords_txt=$keywords_dir/keywords.txt
keywords_int=$keywords_dir/keywords.int
keywords_fst=$keywords_dir/keywords.fsts
keywords_xml=$keywords_dir/kwlist.xml
keywords_log=$keywords_dir/keywords.log
#wfst and am
model_dir=models/song
words=$model_dir/words.txt
wfst=$model_dir/HCLG.fst
am=$model_dir/final.mdl
ali_lex=$model_dir/align_lexicon.int
#lattice out
lat_dir=2.lattice/$dataset_id
mkdir -p $lat_dir
lat_file=$lat_dir/lat.gz
lat_log=$lat_dir/lat.log
#index out
index_dir=3.index/$dataset_id
mkdir -p $index_dir
index_file=$index_dir/index.gz
index_log=$index_dir/index.log
#result out
res_dir=4.result/$dataset_id
mkdir -p $res_dir
res_file=$res_dir/result
res_log=$res_dir/result.log
#evalment
eval_dir=5.eval/$dataset_id
mkdir -p $eval_dir
eval_xml=$eval_dir/kwslist.xml

#######################################
##parmas
#######################################
#params for feature
paste_length_tolerance=2
compress=true

#params for lattice
minimize=true
max_active=7000
min_active=200
beam=8.0
lattice_beam=4.0 # Beam we use in lattice generation.
lat_acwt=0.1  # Just a default value, used for adaptation and beam-pruning..
#params for make index
wip=0.5 #Word insertion penalty
silence_opt= #for silience label
max_expand=180 # limit memory blowup in lattice-align-words
max_states_scale=4
index_lmwt=10
index_acwt=`perl -e "print (1.0/$index_lmwt);"`
max_silence_frames=50
strict=true
max_states=150000
skip_optimization=false     # If you only search for few thousands of keywords, you probablly
                            # can skip the optimization; but if you're going to search for 
                            # millions of keywords, you'd better do set this optimization to 
                            # false and do the optimization on the final index.

#######################################
##0.get feature of pcm
#######################################
if [ ! -f $feats_dir/.done.index ] ; then
    fbank_feats="ark:compute-fbank-feats --verbose=2 --config=$fbank_config scp,p:$wav_scp ark:- |"
    pitch_feats="ark,s,cs:compute-kaldi-pitch-feats --verbose=2 --config=$pitch_config scp,p:$wav_scp ark:- | process-kaldi-pitch-feats --config=$pitch_postprocess_config ark:- ark:- |"
    paste-feats --length-tolerance=$paste_length_tolerance "$fbank_feats" "$pitch_feats" ark:- | \
    copy-feats --compress=$compress ark:- \
    ark,scp:$fbank_ark,$feats_scp >& $feats_log || exit 1;
    
    ! compute-cmvn-stats --spk2utt=ark:$spk2utt scp:$feats_scp ark,scp:$cmvn_ark,$cmvn_scp \
        2> $cmvn_log && echo "Error computing CMVN stats" && exit 1;
    touch $feats_dir/.done.index
fi

#######################################
##1.prepare keyword fst
#######################################
(
 echo '<kwlist ecf_filename="kwlist.xml" language="" encoding="UTF-8" compareNormalize="lowercase" version="" >'
 awk '{ printf("  <kw kwid=\"%s\">\n", $1);
        printf("    <kwtext>"); for (n=2;n<=NF;n++){ printf("%s", $n); if(n<NF){printf(" ");} }
        printf("</kwtext>\n");
        printf("  </kw>\n"); }' < ${keywords_txt}
 echo '</kwlist>'
) > $keywords_xml || exit 1
cat $keywords_txt | sym2int.pl --map-oov 0 -f 2- $words | grep -v " 0 " | grep -v " 0$" > $keywords_int
transcripts-to-fsts ark:$keywords_int ark,t:$keywords_fst >& $keywords_log

#######################################
##2.nnet decoder
#######################################
if [ ! -f $lat_dir/.done.lat ] ; then
    feats="ark,s,cs:apply-cmvn --utt2spk=ark:$utt2spk scp:$cmvn_scp scp:$feats_scp ark:- |"
    nnet-latgen-faster \
     --minimize=$minimize --max-active=$max_active --min-active=$min_active --beam=$beam \
     --lattice-beam=$lattice_beam --acoustic-scale=$lat_acwt --allow-partial=true --word-symbol-table=$words \
     $am $wfst "$feats" "ark:|gzip -c > $lat_file" >& $lat_log || exit 1;
     touch $lat_dir/.done.lat
fi

#######################################
##3.make index
#######################################
if [ ! -f $index_dir/.done.index ] ; then
    lattice-add-penalty --word-ins-penalty=$wip "ark:gzip -cdf $lat_file|" ark:- | \
      lattice-align-words-lexicon $silence_opt --max-expand=$max_expand $ali_lex $am  ark:- ark:- | \
      lattice-scale --acoustic-scale=$index_acwt --lm-scale=$index_lmwt ark:- ark:- | \
      lattice-to-kws-index --max-states-scale=$max_states_scale --allow-partial=true \
      --max-silence-frames=$max_silence_frames --strict=$strict ark:$utter_id ark:- ark:- | \
      kws-index-union --skip-optimization=$skip_optimization --strict=$strict --max-states=$max_states \
      ark:- "ark:|gzip -c > $index_file" >& $index_log || exit 1
    touch $index_dir/.done.index
fi

#######################################
##4.search index
#######################################
for f in $index_file $keywords_fst; do
  [ ! -f $f ] && echo "kws-search: no such file $f" && exit 1;
done
kws-search --strict=$strict --negative-tolerance=-1 \
"ark:gzip -cdf $index_file|" ark:$keywords_fst \
"ark,t:|bin/int2sym.pl -f 2 $utter_id > $res_file" >& $res_log || exit 1;

#######################################
##5.eval
#######################################
#duration=`head -1 $ref_ecf |\
#    grep -o -E "duration=\"[0-9]*[    \.]*[0-9]*\"" |\
#    perl -e 'while($m=<>) {$m=~s/.*\"([0-9.]+)\".*/\1/; print $m/2;}'`
#cat ${res_file} | write_kwslist.pl  --duration=$duration \
#    --normalize=true --duptime=0.6 --remove-dup=true \
#    --digits=3 - ${eval_xml} || exit 1
#KWSEval -e $ref_ecf -r $ref_rttm -t $keywords_xml \
#    -s $eval_xml -c -o -b -d -f ${eval_dir}/${dataset_id} || exit 1;
```


参考：

http://kaldi.sourceforge.net/kws.html




