#ifndef _PROPERTYPARSER_H
#define	_PROPERTYPARSER_H


#define PROPERTY_NAME_LEN 40
#define OBJECT_NAME_LEN 30
#define MAX_BUFFER_LEN 1024



typedef struct {
    char property[PROPERTY_NAME_LEN+1];
    int numValues;
    double *data;
} PROPERTY_STR;

typedef struct {
    char objectName[OBJECT_NAME_LEN+1];
    int numProperties;
    PROPERTY_STR *properties;
} OBJECT_STR;

typedef struct {
    int numObjects;
    OBJECT_STR *listOfObjects;
} OBJECT_LIST_STR;




#endif	/* _PROPERTYPARSER_H */

