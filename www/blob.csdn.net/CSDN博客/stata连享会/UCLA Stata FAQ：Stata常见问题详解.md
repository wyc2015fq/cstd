# UCLA Stata FAQ：Stata常见问题详解 - stata连享会 - CSDN博客





2019年01月03日 08:18:48[arlionn](https://me.csdn.net/arlionn)阅读数：206
所属专栏：[Stata连享会](https://blog.csdn.net/column/details/27605.html)









### [Stata 现场班报名中……](https://gitee.com/arlionn/stata_training/blob/master/README.md)

![](https://upload-images.jianshu.io/upload_images/7692714-8b1fb0b5068487af.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)




### 文章目录
- [[Stata 现场班报名中……](https://gitee.com/arlionn/stata_training/blob/master/README.md)](#Stata_httpsgiteecomarlionnstata_trainingblobmasterREADMEmd_0)
- [Stata Frequently Asked Questions](#Stata_Frequently_Asked_Questions_6)
- [Transferring data to/from Stata](#Transferring_data_tofrom_Stata_10)
- [Stata to HLM](#Stata_to_HLM_21)
- [Stata Graphical User Interface (for Windows)](#Stata_Graphical_User_Interface_for_Windows_29)
- [General Use and Data Management](#General_Use_and_Data_Management_33)
- [Missing values](#Missing_values_65)
- [String variables](#String_variables_71)
- [Error messages](#Error_messages_77)
- [How does Stata compare with SAS and SPSS?](#How_does_Stata_compare_with_SAS_and_SPSS_82)
- [Margins Command](#Margins_Command_95)
- [Regression](#Regression_112)
- [Interactions and comparing coefficients across groups](#Interactions_and_comparing_coefficients_across_groups_114)
- [Regression graphics](#Regression_graphics_125)
- [Types of regression](#Types_of_regression_130)
- [Mediation](#Mediation_151)
- [Logistic Regression](#Logistic_Regression_168)
- [Analysis of Variance](#Analysis_of_Variance_182)
- [SEM](#SEM_209)
- [Graphics](#Graphics_219)
- [Time Series](#Time_Series_236)
- [Capabilities](#Capabilities_241)
- [Advanced Statistics – Survey commands and clustering](#Advanced_Statistics__Survey_commands_and_clustering_245)
- [Advanced Statistics – Other](#Advanced_Statistics__Other_258)
- [Other](#Other_280)
- [关于我们](#_293)
- [联系我们](#_300)
- [往期精彩推文](#_307)




> 
Source: [https://stats.idre.ucla.edu/stata/faq/](https://stats.idre.ucla.edu/stata/faq/)


# Stata Frequently Asked Questions

Can’t find your question? Try checking our [General FAQ](https://stats.idre.ucla.edu/other/mult-pkg/faq/general/) page.

### Transferring data to/from Stata
- [How can I convert files among SAS, SPSS and Stata?](https://stats.idre.ucla.edu/other/mult-pkg/faq/how-do-i-convert-among-sas-stata-and-spss-files/)
- [How can I read data from multiple Excel sheets in Stata?](https://stats.idre.ucla.edu/stata/faq/how-can-i-read-data-from-multiple-excel-sheets-in-stata/)
- [How can I read Stata 12 data files in Stata 11?](https://stats.idre.ucla.edu/stata/faq/how-can-i-read-stata-12-data-files-in-stata-11/)
- [How do I export Stata .dta files to comma-separated files?](https://stats.idre.ucla.edu/stata/faq/how-do-i-export-stata-dta-files-to-comma-separated-files/)
- [How do I export Stata .dta files to a SAS Xport file?](https://stats.idre.ucla.edu/stata/faq/how-do-i-export-a-stata-dta-file-to-a-sas-xport-file/)
- [How can I load, write, or view a dBASE file, Excel file or Access file using odbc?](https://stats.idre.ucla.edu/stata/faq/how-can-i-load-write-or-view-a-dbase-file-excel-file-or-access-file-using-odbc/)
- [How can I convert a Stata file to an MLwiN file using Stata program stata2mlwin?](https://stats.idre.ucla.edu/stata/faq/how-can-i-convert-a-stata-data-file-to-an-mlwin-data-file/)
- [How can I convert a Stata file to an Mplus file?](https://stats.idre.ucla.edu/stata/faq/how-can-i-convert-a-stata-data-file-to-an-mplus-data-file/)

#### Stata to HLM
- [How do I convert a Stata file to HLM?](https://stats.idre.ucla.edu/stata/faq/how-can-i-use-the-hlm-program-to-convert-a-stata-file-to-an-hlm-file/)
- [How can I input a dataset quickly?](https://stats.idre.ucla.edu/stata/faq/how-can-i-input-a-small-dataset-quickly/)
- [How do I read a data file that uses commas/tabs as delimeters?](https://stats.idre.ucla.edu/stata/faq/how-do-i-read-a-data-file-that-has-commas-or-tabs-as-delimeters/)
- [How can I output a formatted ASCII file?](https://stats.idre.ucla.edu/stata/faq/how-can-i-output-a-formatted-ascii-file/)
- [How do I load large data sets (> 1GB) under 32-bit Windows?](http://www.stata.com/support/faqs/win/winmemory.html) (from [Stata FAQs](http://www.stata.com/support/faqs/))

### Stata Graphical User Interface (for Windows)
- [How do I change the font (PC version)?](https://stats.idre.ucla.edu/stata/faq/how-do-i-change-the-font-pc-version/)

### General Use and Data Management
- [How can I anonymize patient IDs and still be able to decode them, if necessary?](https://stats.idre.ucla.edu/stata/faq/how-can-i-anonymize-patient-ids-and-still-be-able-to-decode-them-if-necessary/)
- [How can I sort data within rows independently for each observation?](https://stats.idre.ucla.edu/stata/faq/how-can-i-sort-data-within-rows-independently-for-each-observation/)
- [How can I create all pairs within groups?](https://stats.idre.ucla.edu/stata/faq/how-can-i-create-all-pairs-within-groups/)
- [How do I copy Stata output and Stata graphs into Word?](https://stats.idre.ucla.edu/stata/faq/how-do-i-copy-stata-output-and-stata-graphs-into-word/)
- [How can I save the contents of the results window?](https://stats.idre.ucla.edu/stata/faq/how-can-i-save-the-contents-of-the-results-window/)
- [How can I make the results window hold more results?](https://stats.idre.ucla.edu/stata/faq/how-can-i-make-the-results-window-hold-more-results/)
- [How do I update my copy of Stata?](https://stats.idre.ucla.edu/stata/faq/how-do-i-update-my-copy-of-stata/)
- [How do I check that the same data input by two people are consistently entered?](https://stats.idre.ucla.edu/stata/faq/how-do-i-check-that-the-same-data-input-by-two-people-are-consistently-entered/)
- [How can I list observations in blocks?](https://stats.idre.ucla.edu/stata/faq/how-can-i-list-observations-in-blocks/)
- [Can I do by and sort in one command?](https://stats.idre.ucla.edu/stata/faq/can-i-do-by-and-sort-in-one-command/)
- [Why am I losing precision with large whole numbers (such as an ID variable)?](https://stats.idre.ucla.edu/stata/faq/why-am-i-losing-precision-with-large-whole-numbers-such-as-an-id-variable/)
- [How can I quickly recode continuous variables into groups?](https://stats.idre.ucla.edu/stata/faq/how-can-i-recode-continuous-variables-into-groups/)
- [How do I standardize variables (make them have a mean of 0 and sd of 1)?](https://stats.idre.ucla.edu/stata/faq/how-do-i-standardize-variables-in-stata/)
- [How can I randomly assign observations to groups in Stata?](https://stats.idre.ucla.edu/stata/faq/how-can-i-randomly-assign-observations-to-groups-in-stata/)
- [How can I draw a random sample of my data?](https://stats.idre.ucla.edu/stata/faq/how-can-i-draw-a-random-sample-of-my-data/)
- [How can I read dates with both 2-digit and 4-digit year?](https://stats.idre.ucla.edu/stata/faq/how-can-i-read-dates-with-both-2-digit-and-4-digit-year/)
- [How can I “fill down”/expand observations with respect to dates and then update my dates?](https://stats.idre.ucla.edu/stata/faq/how-can-i-fill-downexpand-observations-with-respect-to-a-time-variable/)
- [How can I count the number of days between two dates?](https://stats.idre.ucla.edu/stata/faq/stata-faq-how-can-i-count-the-number-of-days-between-two-dates/)
- [How do I assign the values of one variable as the value labels for another variable?](https://stats.idre.ucla.edu/stata/faq/how-do-i-assign-the-values-of-one-variable-as-the-value-labels-for-another-variable/)
- [How can I detect duplicate observations?](https://stats.idre.ucla.edu/stata/faq/how-can-i-detect-duplicate-observations-3/)
- [How can I access information stored after I run a command in Stata (returned results)?](https://stats.idre.ucla.edu/stata/faq/how-can-i-access-information-stored-after-i-run-a-command-in-stata-returned-results/)
- [How can I identify cases used by an estimation command using e(sample)?](https://stats.idre.ucla.edu/stata/faq/how-can-i-identify-cases-used-by-an-estimation-command-using-esample/)
- [How can I sample from a datasets with frequency weights?](https://stats.idre.ucla.edu/stata/faq/how-can-i-sample-from-a-dataset-with-frequency-weights/)
- [How do I document and search a Stata dataset?](https://stats.idre.ucla.edu/stata/faq/how-do-i-document-and-search-a-stata-dataset/)
- [How can I create dyad IDs?](https://stats.idre.ucla.edu/stata/faq/how-can-i-create-dyad-ids/)
- [How can I generate automated filenames in Stata?](https://stats.idre.ucla.edu/stata/faq/how-can-i-generate-automated-filenames-in-stata/)
- [How can I merge multiple files in Stata?](https://stats.idre.ucla.edu/stata/faq/how-can-i-merge-multiple-files-in-stata/)
- [How can I combine a large number of files?](https://stats.idre.ucla.edu/stata/faq/how-can-i-combine-a-large-number-of-files/)

### Missing values
- [Can I quickly see how many missing values a variable has?](https://stats.idre.ucla.edu/stata/faq/can-i-quickly-see-how-many-missing-values-a-variable-has/)
- [How can I see the number of missing values and patterns of missing values in my data file?](https://stats.idre.ucla.edu/stata/faq/how-can-i-see-the-number-of-missing-values-and-patterns-of-missing-values-in-my-data-file/)
- [How can I quickly recode missing values into different categories?](https://stats.idre.ucla.edu/stata/faq/how-can-i-recode-missing-values-into-different-categories/)
- [How can I use multiply imputed data where original data is not included?](https://stats.idre.ucla.edu/stata/faq/how-can-i-use-multiply-imputed-data-where-original-data-is-not-included/)

### String variables
- [How can I quickly convert many string variables to numeric variables?](https://stats.idre.ucla.edu/stata/faq/how-can-i-quickly-convert-many-string-variables-to-numericvariables/)
- [How can I turn a string variable containing dates into a date variable Stata can recognize?](https://stats.idre.ucla.edu/stata/faq/how-can-i-turn-a-string-variable-containing-dates-into-a-date-variable-stata-can-recognize/)
- [How can I extract a portion of a string variable using regular expressions?](https://stats.idre.ucla.edu/stata/faq/how-can-i-extract-a-portion-of-a-string-variable-using-regular-expressions/)

### Error messages
- [How can I handle the matsize too small error?](https://stats.idre.ucla.edu/stata/faq/how-can-i-handle-the-matsize-too-small-error/)
- [How can I handle the No Room to Add Observations error?](https://stats.idre.ucla.edu/stata/faq/how-can-i-handle-the-no-room-to-add-more-observations-error/)

### How does Stata compare with SAS and SPSS?
- [How do I do this SAS Command in Stata?](https://stats.idre.ucla.edu/stata/faq/how-do-i-do-this-sas-command-in-stata/)
- [How do I do this SPSS Command in Stata?](https://stats.idre.ucla.edu/stata/faq/how-do-i-do-this-spss-command-in-stata/)

General Statistics
- [How can you get 3-way, 4-way, 5-way or more cross tabulations in Stata?](https://stats.idre.ucla.edu/stata/faq/how-can-you-get-3-way-4-way-5-way-or-more-cross-tabulation-in-stata/)
- [How can I compute power for contingency tables in Stata?](https://stats.idre.ucla.edu/stata/faq/how-can-i-compute-power-for-contingency-tables-in-stata/)
- [How can I get descriptive statistics and the five number summary on one line?](https://stats.idre.ucla.edu/stata/faq/how-can-i-get-descriptive-statistics-and-the-five-number-summary-on-one-line/)
- [What’s with the different formulas for kurtosis?](https://stats.idre.ucla.edu/other/mult-pkg/faq/general/faq-whats-with-the-different-formulas-for-kurtosis/)
- [What are the differences between one-tailed and two-tailed tests?](https://stats.idre.ucla.edu/other/mult-pkg/faq/general/faq-what-are-the-differences-between-one-tailed-and-two-tailed-tests/)

### Margins Command
- [How can I get margins for a multiply imputed survey logit model?](https://stats.idre.ucla.edu/stata/faq/how-can-i-get-margins-for-a-multiply-imputed-survey-logit-model/)
- [How can I get margins and marginsplot on multiply imputated data?](https://stats.idre.ucla.edu/stata/faq/how-can-i-get-margins-and-marginsplot-with-multiply-imputed-data/)
- [How can I get anova type results from xtmixed using the margins command?](https://stats.idre.ucla.edu/stata/faq/how-can-i-get-anova-type-results-from-xtmixed-using-the-margins-command-stata/)
- [How can I use the margins command to understand multiple interactions in regression and anova?](https://stats.idre.ucla.edu/stata/faq/how-can-i-use-the-margins-command-to-understand-multiple-interactions-in-regression-and-anova-stata-11/)
- [More of how can I use the margins command to understand multiple interactions in regression?](https://stats.idre.ucla.edu/stata/faq/more-of-how-can-i-use-the-margins-command-to-understand-multiple-interactions-in-regression-stata/)
- [How can I use the margins command to understand multiple interactions in logistic regression?](https://stats.idre.ucla.edu/stata/faq/how-can-i-use-the-margins-command-to-understand-multiple-interactions-in-logistic-regression-stata/)
- [How can I graph the results of the margins command?](https://stats.idre.ucla.edu/stata/faq/mar-graph/how-can-i-graph-the-results-of-the-margins-command-stata/)
- [How can I use the margins command with a 3-way anova interaction?](https://stats.idre.ucla.edu/stata/faq/how-can-i-use-the-margins-command-with-a-3-way-anova-interaction-stata/)
- [How can I get anova simple main effects with the margins command?](https://stats.idre.ucla.edu/stata/faq/how-can-i-get-anova-simple-main-effects-with-the-margins-command-stata/)
- [How can I do anova contrasts using the margins command?](https://stats.idre.ucla.edu/stata/faq/how-can-i-do-anova-contrasts-using-the-margins-command-stata/)
- [How can get anova main-effects with dummy coding using margins?](https://stats.idre.ucla.edu/stata/faq/how-can-get-anova-main-effects-with-dummy-coding-using-margin-stata/)
- [Why doesn’t the margins command work with a nested factor in anova?](https://stats.idre.ucla.edu/stata/faq/why-doesnt-the-margins-command-work-with-a-nested-factor-in-anova-stata/)
- [How can I do simple main effects using anovalator?](https://stats.idre.ucla.edu/stata/faq/how-can-i-do-simple-main-effects-using-anovalator-stata/)
- [How do I use the anovalator command?](https://stats.idre.ucla.edu/stata/faq/how-do-i-use-the-anovalator-command-stata/)

### Regression

#### Interactions and comparing coefficients across groups
- [Everything you always wanted to know about contrasts* (*But were afraid to ask)](https://stats.idre.ucla.edu/stata/faq/everything-you-always-wanted-to-know-about-contrasts-but-were-afraid-to-ask/)
- [How can I understand a 3-way continuous interaction? (Stata 12)](https://stats.idre.ucla.edu/stata/faq/how-can-i-understand-a-3-way-continuous-interaction-stata-12/)
- [How can I understand a continuous by continuous interaction? (Stata 12)](https://stats.idre.ucla.edu/stata/faq/how-can-i-explain-a-continuous-by-continuous-interaction-stata-12/)
- [How can I understand a categorical by continuous interaction? (Stata 12)](https://stats.idre.ucla.edu/stata/faq/how-can-i-understand-a-categorical-by-continuous-interaction-stata-12/)
- [What happens if you omit the main effect in a regression model with an interaction?](https://stats.idre.ucla.edu/stata/faq/what-happens-if-you-omit-the-main-effect-in-a-regression-model-with-an-interaction/)
- [Why don’t my anova and regression results agree? (Stata 11)](https://stats.idre.ucla.edu/stata/faq/why-dont-my-anova-and-regression-results-agree-stata-11/)
- [How can I get anova main-effects with dummy coding? (Stata 10)](https://stats.idre.ucla.edu/stata/faq/how-can-get-anova-main-effects-with-dummy-coding-stata-version-10-and-earlier/)
- [How can I compare regression coefficients between 2 groups?](https://stats.idre.ucla.edu/stata/faq/how-can-i-compare-regression-coefficients-between-2-groups/)
- [How can I compare regression coefficients across 3 (or more) groups?](https://stats.idre.ucla.edu/stata/faq/how-can-i-compare-regression-coefficients-across-3-or-more-groups/)

#### Regression graphics
- [How can graphically compare OLS and BLUP results in Stata?](https://stats.idre.ucla.edu/stata/faq/how-can-i-graphically-compare-ols-and-blup-results-in-stata/)
- [How can I do a scatterplot with regression line in Stata?](https://stats.idre.ucla.edu/stata/faq/how-can-i-do-a-scatterplot-with-regression-line-in-stata/)

#### Types of regression
- [How can I get an R2 with robust regression (rreg)?](https://stats.idre.ucla.edu/stata/faq/how-can-i-get-an-r2-with-robust-regression-rreg/)
- [How do I interpret quantile regression coefficients?](https://stats.idre.ucla.edu/stata/faq/how-do-i-interpret-quantile-regression-coefficients/)
- [How does one do regression when the dependent variable is a proportion?](https://stats.idre.ucla.edu/stata/faq/how-does-one-do-regression-when-the-dependent-variable-is-a-proportion/)
- [What is seemingly unrelated regression and how can I perform it in Stata?](https://stats.idre.ucla.edu/stata/faq/what-is-seemingly-unrelated-regression-and-how-can-i-perform-it-in-stata/)
- [How can I analyze count data in Stata?](https://stats.idre.ucla.edu/stata/faq/how-can-i-analyze-count-data-in-stata/)
- [How can I use countfit in choosing a count model?](https://stats.idre.ucla.edu/stata/faq/how-can-i-use-countfit-in-choosing-a-count-model/)
- [What is the difference between xtreg-re, xtreg-fe?](https://stats.idre.ucla.edu/stata/faq/what-is-the-difference-between-xtreg-re-xtreg-fe/)
- [How can I estimate effect size for mixed?](https://stats.idre.ucla.edu/stata/faq/how-can-i-estimate-effect-size-for-mixed/)
- [How can I compute effect size in Stata for regression?](https://stats.idre.ucla.edu/stata/faq/how-can-i-compute-effect-size-in-stata-for-regression/)
- [How can I generate fungible regression weights?](https://stats.idre.ucla.edu/stata/faq/fungible/how-can-i-generate-fungible-regression-weights/)
- [Comparing various methods of analyzing clustered data in Stata.](https://stats.idre.ucla.edu/stata/faq/what-are-the-some-of-the-methods-for-analyzing-clustered-data-in-stata/)
- [How can I create dummy variables in Stata?](https://stats.idre.ucla.edu/stata/faq/how-can-i-create-dummy-variables-in-stata/)
- [Can I make regression tables that look like those in journal articles?](https://stats.idre.ucla.edu/stata/faq/how-can-i-use-estout-to-make-regression-tables-that-look-like-those-in-journal-articles/)
- [How can I run a piecewise regression in Stata?](https://stats.idre.ucla.edu/stata/faq/how-can-i-run-a-piecewise-regression-in-stata/)
- [How can I find where to split a piecewise regression?](https://stats.idre.ucla.edu/stata/faq/how-can-i-find-where-to-split-a-piecewise-regression/)
- [How can I perform the likelihood ratio, Wald, and Lagrange multiplier (score) test in Stata?](https://stats.idre.ucla.edu/stata/faq/how-can-i-perform-the-likelihood-ratio-wald-and-lagrange-multiplier-score-test-in-stata/)
- [How Can I Perform Post Estimation Tests with Multiply Imputed Datasets?](https://stats.idre.ucla.edu/stata/faq/how-can-i-perform-post-estimation-tests-with-multiply-imputed-datasets/)
- [How Can I Estimate R-squared for a Model Estimated Using Multiply Imputed Data?](https://stats.idre.ucla.edu/stata/faq/how-can-i-estimate-r-squared-for-a-model-estimated-with-multiply-imputed-data/)

### Mediation
- [How can I compute indirect effects with imputed data? (Method 2)](https://stats.idre.ucla.edu/stata/faq/how-can-i-compute-indirect-effects-with-imputed-data-method-2/)
- [How can I compute indirect effects with imputed data? (Method 1)](https://stats.idre.ucla.edu/stata/faq/how-can-i-compute-indirect-effects-with-imputed-data-method-1/)
- [How can I get Monte Carlo standard errors for indirect effects?](https://stats.idre.ucla.edu/stata/faq/how-can-i-get-monte-carlo-standard-errors-for-indirect-effects/)
- [How can I compare indirect effects in a multiple group model? (Stata 12)](https://stats.idre.ucla.edu/stata/faq/how-can-i-compare-indirect-effects-in-a-multiple-group-model/)
- [How can I do mediation analysis with the sem command? (Stata 12)](https://stats.idre.ucla.edu/stata/faq/how-can-i-do-mediation-analysis-with-the-sem-command/)
- [How can I perform mediation with multilevel data? (Method 2)](https://stats.idre.ucla.edu/stata/faq/how-can-i-perform-mediation-with-multilevel-data-method-2/)
- [How can I perform mediation with multilevel data? (Method 1)](https://stats.idre.ucla.edu/stata/faq/how-can-i-perform-mediation-with-multilevel-data-method-1/)
- [How can I perform mediation with binary variables?](https://stats.idre.ucla.edu/stata/faq/how-can-i-perform-mediation-with-binary-variables/)
- [How can I do mediation analysis with a categorical IV in Stata?](https://stats.idre.ucla.edu/stata/faq/how-can-i-do-mediation-analysis-with-a-categorical-iv-in-stata/)
- [How can I do moderated mediation in Stata?](https://stats.idre.ucla.edu/stata/faq/how-can-i-do-moderated-mediation-in-stata/)
- [How can I do moderated mediation with a categorical moderator using sem?](https://stats.idre.ucla.edu/stata/faq/how-can-i-do-moderated-mediation-with-a-categorical-moderator-using-sem/)
- [How can I do moderated mediation with a categorical moderator in Stata?](https://stats.idre.ucla.edu/stata/faq/how-can-i-do-moderated-mediation-with-a-categorical-moderator-in-stata/)
- [How can I perform Sobel-Goodman mediation tests?](https://stats.idre.ucla.edu/stata/faq/how-to-perform-sobel-goodman-mediation-tests-in-stata/)
- [How can I analyze multiple mediators in Stata?](https://stats.idre.ucla.edu/stata/faq/how-can-i-analyze-multiple-mediators-in-stata/)

### Logistic Regression
- [How can I understand a continuous by continuous interaction in logistic regression?](https://stats.idre.ucla.edu/stata/faq/how-can-i-understand-a-continuous-by-continuous-interaction-in-logistic-regression-stata-12/)
- [How can I understand a categorical by continuous interaction in logistic regression?](https://stats.idre.ucla.edu/stata/faq/how-can-i-understand-a-categorical-by-continuous-interaction-in-logistic-regression-stata-12/)
- [How can I understand a categorical by categorical interaction in logistic regression?](https://stats.idre.ucla.edu/stata/faq/how-can-i-understand-a-categorical-by-categorical-interaction-in-logistic-regression-stata-12/)
- [How can I compute predictive margins for xtmelogit with random effects?](https://stats.idre.ucla.edu/stata/faq/how-can-i-compute-predictive-margins-for-xtmelogit-with-random-effects/)
- [How can I estimate probabilities that include the random effects in xtmelogit? (Stata 11)](https://stats.idre.ucla.edu/stata/faq/how-can-i-estimate-probabilities-that-include-the-random-effects-in-xtmelogit/)
- [How can I understand a categorical by continuous interaction in ologit?](https://stats.idre.ucla.edu/stata/faq/how-can-i-understand-a-categorical-by-continuous-interaction-in-ologit/)
- [How do I interpret odds ratios in logistic regression?](https://stats.idre.ucla.edu/stata/faq/how-do-i-interpret-odds-ratios-in-logistic-regression/)
- [How do I use adjust in probit or logit?](https://stats.idre.ucla.edu/stata/faq/how-do-i-use-adjust-in-probit-or-logit/)
- [How can I get a Somers’ D after logistic regression in Stata?](https://stats.idre.ucla.edu/stata/faq/how-can-i-get-a-somers-d-after-logistic-regression-in-stata/)
- [How can I estimate relative risk using glm for common outcomes in cohort studies?](https://stats.idre.ucla.edu/stata/faq/how-can-i-estimate-relative-risk-using-glm-for-common-outcomes-in-cohort-studies/)
- [How can I test the difference in area under ROC curve for two logistic models?](https://stats.idre.ucla.edu/stata/faq/how-can-i-test-the-difference-in-area-under-roc-curve-for-two-logistic-regression-models/)

### Analysis of Variance
- [Everything you always wanted to know about contrasts* (*But were afraid to ask)](https://stats.idre.ucla.edu/stata/faq/everything-you-always-wanted-to-know-about-contrasts-but-were-afraid-to-ask/)
- [How can I get denominator degrees of freedom for xtmixed?](https://stats.idre.ucla.edu/stata/faq/how-can-i-get-denominator-degrees-of-freedom-for-xtmixed/)
- [How can I test simple effects in repeated measures models?](https://stats.idre.ucla.edu/stata/faq/how-can-i-test-simple-effects-in-repeated-measures-models-stata-12/) (Stata 12)
- [How can I understand a three-way interaction in ANOVA? (Stata 12)](https://stats.idre.ucla.edu/stata/faq/how-can-i-understand-a-three-way-interaction-in-anova-stata-12/)
- [How can I do power and robustness analyses for factorial ANOVA? (Stata 11)](https://stats.idre.ucla.edu/stata/faq/how-can-i-do-power-and-robustness-analyses-for-factorial-anova-stata-11/)
- [How can I get ANOVA main-effects with dummy coding?](https://stats.idre.ucla.edu/stata/faq/how-can-get-anova-main-effects-with-dummy-coding-stata-version-10-and-earlier/)
- [How can I get ANOVA type III results using xtmixed?](https://stats.idre.ucla.edu/stata/faq/how-can-i-get-anova-type-iii-results-using-xtmixed/)
- [How can I get ANOVA simple main effects with dummy coding?](https://stats.idre.ucla.edu/stata/faq/how-can-i-get-anova-simple-main-effects-with-dummy-coding/)
- [Why don’t my ANOVA and regression results agree? (Stata 11)](https://stats.idre.ucla.edu/stata/faq/why-dont-my-anova-and-regression-results-agree-stata-11/)
- [How can I analyze a nested model using xtmixed?](https://stats.idre.ucla.edu/stata/faq/how-can-i-analyze-a-nested-model-using-xtmixed/)
- [How can I compute critical values for test of simple effects?](https://stats.idre.ucla.edu/stata/faq/how-can-i-compute-critical-values-for-test-of-simple-effects/)
- [How can I do classical ANOVA designs using xtmixed? (Stata 12)](https://stats.idre.ucla.edu/stata/faq/how-can-i-do-classical-anova-designs-using-xtmixed-stata-12/)
- [How can I do post-hoc pairwise comparisons in Stata?](https://stats.idre.ucla.edu/stata/faq/faqhow-can-i-do-post-hoc-pairwise-comparisons-using-stata/)
- [How can I do post-hoc pairwise comparisons of adjusted means in Stata?](https://stats.idre.ucla.edu/stata/faq/faq-how-can-i-do-post-hoc-pairwise-comparisons-of-adjusted-means-in-stata/)
- [How can I do profile analysis in Stata?](https://stats.idre.ucla.edu/stata/faq/how-can-i-do-profile-analysis-in-stata/)
- [How can I do multivariate repeated measures in Stata?](https://stats.idre.ucla.edu/stata/faq/how-can-i-do-multivariate-repeated-measures-in-stata/)
- [How can I plot ANOVA cell means in Stata?](https://stats.idre.ucla.edu/stata/faq/how-can-i-plot-anova-cell-means-in-stata/)
- [How can graph group means and standard deviations for ANOVA?](https://stats.idre.ucla.edu/stata/faq/how-can-graph-group-means-and-standard-deviations-for-anova/)
- [How can I do ANOVA contrasts in Stata?](https://stats.idre.ucla.edu/stata/faq/how-can-i-do-anova-contrasts-in-stata/)
- [How can I do tests of simple main effects in Stata?](https://stats.idre.ucla.edu/stata/faq/how-can-i-do-tests-of-simple-main-effects-in-stata/)
- [How can I check for homogeneity of variance in a factorial ANOVA design?](https://stats.idre.ucla.edu/stata/faq/how-can-i-check-for-homogeneity-of-variance-in-a-factorial-anova-design/)
- [How can I test for nonadditivity in a randomized block ANOVA in Stata?](https://stats.idre.ucla.edu/stata/faq/how-can-i-test-for-nonadditivity-in-a-randomized-block-anova-in-stata/)
- [How can I determine the correct term in an ANOVA using Stata?](https://stats.idre.ucla.edu/stata/faq/faqhow-can-i-determine-the-correct-term-in-an-anova-using-stata/)
- [How does the ANOVA command handle collinearity?](http://www.stata.com/support/faqs/stat/anova.html) (from [Stata FAQs](http://www.stata.com/support/faqs/))

### SEM
- [Linear growth models: xtmixed versus sem](https://stats.idre.ucla.edu/stata/faq/linear-growth-models-xtmixed-vs-sem/)
- [How can I do CFA with binary variables?](https://stats.idre.ucla.edu/stata/faq/how-can-i-do-cfa-with-binary-variables/)
- [What are the saturated and baseline models in sem?](https://stats.idre.ucla.edu/stata/faq/what-are-the-saturated-and-baseline-models-in-sem/)
- [How can I do mediation analysis with the sem command? (Stata 12)](https://stats.idre.ucla.edu/stata/faq/how-can-i-do-mediation-analysis-with-the-sem-command/)
- [How can I check measurement invariance using the sem command? (Stata 12)](https://stats.idre.ucla.edu/stata/faq/how-can-i-check-measurement-invariance-using-the-sem-command-stata-12/)
- [How can I do EFA within a CFA framework? (Stata 12)](https://stats.idre.ucla.edu/stata/faq/how-can-i-do-efa-within-a-cfa-framework-stata-12/)
- [How can I perform a factor analysis with categorical (or categorical and continuous) variables?](https://stats.idre.ucla.edu/stata/faq/how-can-i-perform-a-factor-analysis-with-categorical-or-categorical-and-continuous-variables/)

### Graphics
- [How can I overlay two histograms?](https://stats.idre.ucla.edu/stata/faq/how-can-i-overlay-two-histograms/)
- [How can I make a correlation matrix heat map?](https://stats.idre.ucla.edu/stata/faq/how-can-i-make-a-correlation-matrix-heat-map/)
- [How can I graph chi-square power curves in Stata?](https://stats.idre.ucla.edu/stata/faq/how-can-i-graph-chi-square-power-curves-in-stata/)
- [Where can I find examples of user written graph commands for Stata?](https://stats.idre.ucla.edu/stata/faq/where-can-i-find-examples-of-user-written-graph-commands-for-stata/)
- [How can I do a profile plot in Stata?](https://stats.idre.ucla.edu/stata/faq/how-can-i-do-a-profile-plot-in-stata/)
- [How can graph group means and standard deviations for ANOVA?](https://stats.idre.ucla.edu/stata/faq/how-can-graph-group-means-and-standard-deviations-for-anova/)
- [How can I combine a histogram and a boxplot in Stata?](https://stats.idre.ucla.edu/stata/faq/how-can-i-combine-a-histogram-and-a-boxplot-in-stata/)
- [How can I make a bar graph with error bars?](https://stats.idre.ucla.edu/stata/faq/how-can-i-make-a-bar-graph-with-error-bars/)
- [How can I graph two (or more) groups using different symbols?](https://stats.idre.ucla.edu/stata/faq/how-can-i-graph-two-or-more-groups-using-different-symbols/)
- [How do I graph data with dates?](https://stats.idre.ucla.edu/stata/faq/how-do-i-graph-data-with-dates/)
- [How can I view/compare different marker symbol options?](https://stats.idre.ucla.edu/stata/faq/how-can-i-view-different-marker-symbol-options/)
- [How do I save a Stata graph as an EPS file?](https://stats.idre.ucla.edu/stata/faq/how-do-i-save-a-stata-graph-as-an-eps-file/)
- [How do I use xtline in Stata?](https://stats.idre.ucla.edu/stata/faq/how-do-i-use-xtline-in-stata/)
- [How can I visualize longitudinal data in Stata?](https://stats.idre.ucla.edu/stata/faq/how-can-i-visualize-longitudinal-data-in-stata/)

### Time Series
- [How to collapse a daily time series to a monthly time series](https://stats.idre.ucla.edu/stata/faq/how-can-i-collapse-a-daily-time-series-to-a-monthly-time-series/)
- [How to extract month and year component from a variable with %tm format](https://stats.idre.ucla.edu/stata/faq/how-can-i-extract-month-and-year-component-from-a-variable-with-tm-format/)

### Capabilities
- See the [Stata capabilities page](http://www.stata.com/capabilities/) for information about the capabilities of Stata, including [Linear models](http://www.stata.com/capabilities/linear.html), [Binary, count, and limited dependent variables](http://www.stata.com/capabilities/binary.html), [Resampling and simulation methods](http://www.stata.com/capabilities/bootstrap.html), [Graphics](http://www.stata.com/capabilities/graphics.html), [Survey methods](http://www.stata.com/capabilities/svy.html), [Survival analysis](http://www.stata.com/capabilities/survival.html)

### Advanced Statistics – Survey commands and clustering
- [How do I use the Stata survey (svy) commands?](https://stats.idre.ucla.edu/stata/faq/how-do-i-use-the-stata-survey-svy-commands/)
- [Sample setups for commonly used survey data sets](https://stats.idre.ucla.edu/other/mult-pkg/faq/sample-setups-for-commonly-used-survey-data-sets)
- [Choosing the correct analysis for various survey designs](https://stats.idre.ucla.edu/other/mult-pkg/faq/faq-choosing-the-correct-analysis-for-various-survey-designs/)
- [How can I check for collinearity in survey regression?](https://stats.idre.ucla.edu/stata/faq/how-can-i-check-for-collinearity-in-survey-regression/)
- [How can I do a t-test with survey data?](https://stats.idre.ucla.edu/stata/faq/how-can-i-do-a-t-test-with-survey-data/)
- [How can the standard errors with the cluster() option be smaller than those without the cluster() option?](http://www.stata.com/support/faqs/stat/cluster.html) (from [Stata FAQs](http://www.stata.com/support/faqs/))
- [Do the svy commands handle zero weights differently than non-svy commands?](http://www.stata.com/support/faqs/stat/zerowgt.html) (from [Stata FAQs](http://www.stata.com/support/faqs/))
- [How can I analyze a subpopulation of my survey data in Stata?](https://stats.idre.ucla.edu/stata/faq/how-can-i-analyze-a-subpopulation-of-my-survey-data-in-stata/)
- [Why doesn’t summarize accept pweights? What does summarize calculate when you use aweights?](http://www.stata.com/support/faqs/stat/supweight.html) (from [Stata FAQs](http://www.stata.com/support/faqs/))
- [Why doesn’t the test of the overall survey regression model in Stata match the results from SAS and SUDAAN?](https://stats.idre.ucla.edu/stata/faq/adjusted_wald_test/)

### Advanced Statistics – Other
- How can I do multilevel principal components analysis?
- [How can I access the random effects after xtmixed using diparm?](https://stats.idre.ucla.edu/stata/faq/how-can-i-access-the-random-effects-after-xtmixed-using-_diparm/)
- [How can I convert a multilevel model to a mixed model?](https://stats.idre.ucla.edu/stata/faq/how-can-i-convert-a-multilevel-model-to-a-mixed-model/)
- [How can I do factor analysis with missing data in Stata?](https://stats.idre.ucla.edu/stata/faq/how-can-i-do-factor-analysis-with-missing-data-in-stata/)
- [How can I do correspondence analysis on summary data?](https://stats.idre.ucla.edu/stata/faq/how-can-i-do-correspondence-analysis-on-summary-data/)
- [How to do parallel analysis for pca or factor analysis in Stata?](https://stats.idre.ucla.edu/stata/faq/how-to-do-parallel-analysis-for-pca-or-factor-analysis-in-stata/)
- [How can I compute tetrachoric correlations?](https://stats.idre.ucla.edu/stata/faq/how-can-i-compute-tetrachoric-correlations-in-stata/)
- [How can I calculate a kappa statistic for variables with unequal score ranges?](https://stats.idre.ucla.edu/stata/faq/how-can-i-calculate-a-kappa-statistic-for-variables-with-unequal-score-ranges/)
- [How should I analyze percentile rank data?](https://stats.idre.ucla.edu/stata/faq/how-should-i-analyze-percentile-rank-data/)
- [Why do estimation commands sometimes drop variables?](http://www.stata.com/support/faqs/stat/drop.html)  (from [Stata FAQs](http://www.stata.com/support/faqs/))
- [How can I test for equality of distribution?](https://stats.idre.ucla.edu/stata/faq/how-can-i-test-for-equality-of-distribution/)
- [How can I test for a trend across a categorical variable?](https://stats.idre.ucla.edu/stata/faq/how-can-i-test-for-a-trend-across-a-categorical-variable/)
- [How do I write my own bootstrap program?](https://stats.idre.ucla.edu/stata/faq/how-do-i-write-my-own-bootstrap-program/)
- [How can I manually generate the predicted counts from a ZIP or ZINB model based on the parameter estimates?](https://stats.idre.ucla.edu/stata/faq/how-can-i-manually-generate-the-predicted-counts-from-a-zip-or-zinb-model-based-on-the-parameter-estimates/)
- [How can I perform multiple imputation on longitudinal data using ICE?](https://stats.idre.ucla.edu/stata/faq/how-can-i-perform-multiple-imputation-on-longitudinal-data-using-ice/)
- [How can I calculate Moran’s I in Stata?](https://stats.idre.ucla.edu/stata/faq/how-can-i-calculate-morans-i-in-stata/)
- [How do I generate a variogram for spatial data in Stata?](https://stats.idre.ucla.edu/stata/faq/how-do-i-generate-a-variogram-for-spatial-data-in-stata/)
- [How can I fit a random intercept or mixed effects model with heteroskedastic errors in Stata?](https://stats.idre.ucla.edu/stata/faq/how-can-i-fit-a-random-intercept-or-mixed-effects-model-with-heteroskedastic-errors-in-stata/)
- [How is the 95% CI of the variance component in a mixed model calculated?](https://stats.idre.ucla.edu/stata/faq/how-is-the-95-ci-of-the-variance-component-in-a-mixed-model-calculated/)

### Other
- [What is the difference between = and == in STATA?](https://stats.idre.ucla.edu/stata/faq/what-is-the-difference-between-and-in-stata/)
- [What types of weights do SAS, Stata and SPSS support?](https://stats.idre.ucla.edu/other/mult-pkg/faq/what-types-of-weights-do-sas-stata-and-spss-support/)
- [How do I do elementwise operations on a matrix?](https://stats.idre.ucla.edu/stata/faq/how-do-i-do-elementwise-operations-on-a-matrix/)
- [How can I get poisson probabilities in Stata?](https://stats.idre.ucla.edu/stata/faq/how-can-i-get-poisson-probabilities-in-stata/)
- [How can I generate a saturated model using DESMAT?](https://stats.idre.ucla.edu/stata/faq/how-to-generate-a-saturated-model-using-desmat/)
- [How can I translate a file into a Postscript file?](https://stats.idre.ucla.edu/stata/faq/how-can-i-translate-a-file-into-a-postscript-file/)
- [How do I use file write to write results or other information to an external text file?](https://stats.idre.ucla.edu/stata/faq/how-do-i-use-file-write-to-write-results-or-other-information-to-an-external-text-file/)
- [How do I use file read to read in information in an external text file?](https://stats.idre.ucla.edu/stata/faq/how-do-i-use-file-read-to-read-in-information-in-an-external-text-file/)
- [How do I use search to search for programs and additional help?](https://stats.idre.ucla.edu/stata/faq/search-faq/)

> 
#### 关于我们

- 【**Stata 连享会(公众号：StataChina)**】由中山大学连玉君老师团队创办，旨在定期与大家分享 Stata 应用的各种经验和技巧。
- 公众号推文同步发布于 [CSDN-Stata连享会](https://blog.csdn.net/arlionn) 、[简书-Stata连享会](http://www.jianshu.com/u/69a30474ef33) 和 [知乎-连玉君Stata专栏](https://www.zhihu.com/people/arlionn)。可以在上述网站中搜索关键词`Stata`或`Stata连享会`后关注我们。
- 点击推文底部【阅读原文】可以查看推文中的链接并下载相关资料。
- Stata连享会 [精彩推文1](https://gitee.com/arlionn/stata_training/blob/master/README.md)  || [精彩推文2](https://github.com/arlionn/stata/blob/master/README.md)

> 
#### 联系我们

- **欢迎赐稿：** 欢迎将您的文章或笔记投稿至`Stata连享会(公众号: StataChina)`，我们会保留您的署名；录用稿件达`五篇`以上，即可**免费**获得 Stata 现场培训 (初级或高级选其一) 资格。
- **意见和资料：** 欢迎您的宝贵意见，您也可以来信索取推文中提及的程序和数据。
- **招募英才：** 欢迎加入我们的团队，一起学习 Stata。合作编辑或撰写稿件五篇以上，即可**免费**获得 Stata 现场培训 (初级或高级选其一) 资格。
- **联系邮件：**[StataChina@163.com](mailto:StataChina@163.com)

> 
#### 往期精彩推文

- [Stata连享会推文列表1](https://www.jianshu.com/p/de82fdc2c18a)
- [Stata连享会推文列表2](https://gitee.com/arlionn/jianshu/blob/master/README.md)
- Stata连享会 [精彩推文1](https://gitee.com/arlionn/stata_training/blob/master/README.md)  || [精彩推文2](https://github.com/arlionn/stata/blob/master/README.md)

![](https://upload-images.jianshu.io/upload_images/7692714-8b1fb0b5068487af.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![欢迎加入Stata连享会(公众号: StataChina)](http://upload-images.jianshu.io/upload_images/7692714-0744d6aa74a821e6.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)



