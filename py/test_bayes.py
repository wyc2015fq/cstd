from sklearn.datasets import fetch_20newsgroups
import numpy as np
from sklearn.naive_bayes import MultinomialNB
from sklearn import metrics,cross_validation
from sklearn.feature_extraction.text import TfidfVectorizer
cats = ['alt.atheism', 'sci.med']
newsgroups= fetch_20newsgroups(subset='all',remove=('headers',
'footers', 'quotes'), categories=cats)

vectorizer = TfidfVectorizer()
vectors = vectorizer.fit_transform(newsgroups.data)

clf = MultinomialNB(alpha=.01)
print("CrossValidation Score: ",
 np.mean(cross_validation.cross_val_score(clf,vectors, newsgroups.target, scoring='f1')))
