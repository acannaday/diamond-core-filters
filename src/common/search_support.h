#ifndef	_SEARCH_SUPPORT_H_
#define	_SEARCH_SUPPORT_H_	1

#include <stdint.h>
#include "face.h"
#include "rgb.h"
#include "fil_histo.h"
#include <opencv/cv.h>

#ifdef __cplusplus
extern "C" {
#endif

/* external declaration to create new search  of specified type */
img_search * create_search(search_types_t type, const char *name);

int search_exists(const char *name, search_set *set);
void search_add_list(img_search *new_search, search_set *set);


	
#ifdef __cplusplus
}
#endif

#endif	/* ! _SEARCH_SUPPORT_H_ */