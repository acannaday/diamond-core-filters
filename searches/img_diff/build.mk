FILTERS  += searches/img_diff/fil_img_diff
SEARCHES += searches/img_diff/img_diff.search

searches_img_diff_fil_img_diff_SOURCES = \
	searches/img_diff/fil_img_diff.c \
	searches/img_diff/fil_img_diff.h

searches_img_diff_fil_img_diff_LDADD = $(LDADD) -lm