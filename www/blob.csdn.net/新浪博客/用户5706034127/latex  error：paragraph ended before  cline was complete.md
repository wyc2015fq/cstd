# latex  error：paragraph ended before \cline was complete_用户5706034127_新浪博客
|||
注意区别hline和cline
\begin{table}[tp]
 \centering
 \fontsize{6.5}{8}\selectfont
 \caption{Performance comparison of models
on key words queries and verbose queries.}
 \label{tab:performance_comparison}
 \begin{tabular}{|c|c
|c|c|c| c|c|c|c|}
 \hline
\multicolumn{2}{|c|}{Models}&JM-LM&Dir-LM&TS-LM&HF-Wiki-LM&SF-Wiki-LM&HF-Wiki-LM&SF-Wiki-LM\cr\hline
 \hline
\multirow{2}{*}{MAP@100}
 &Key
words&0.3013&0.3814&0.4413
 &0.5809&0.1037
 &0.1709&0.3013\cr\cline{2-9}
&Verbose&0.3013&0.3814&0.4413
 &0.5809&0.1037
 &0.1709&0.3013\cr\cline
 \end{tabular}
\end{table}
\end{document} 
