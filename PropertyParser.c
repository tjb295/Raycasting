/*
 Include file for Property Parser
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <regex.h>
#include <ctype.h>
#include "PropertyParser.h"

#define MAX_MATCH 20
#define NUM_REG_TYPES 1

OBJECT_STR *getPropertyValues(OBJECT_STR *_object, char *_result);

OBJECT_LIST_STR *getProperties(char *_filename) {

    // char *regPattern[] = {"[\\s]*[a-zA-z]+:[\\s]*\\[[0-9,\\.\\s]+]", "[\\s]*[a-zA-z]+:[\\s]*[0-9\\.]+"};
    char *regPattern = "[a-zA-z]+:[ ]*\\[[-0-9,\\. ]+]|[a-zA-z]+:[ ]*[-0-9\\.]+";

    regex_t regexComp;
    regmatch_t match;


    if (_filename == NULL || strlen(_filename) == 0) {
        fprintf(stderr, "Filename not given\n");
        return NULL;
    }
    FILE *fp = fopen(_filename, "r");
    if (fp == NULL) {
        perror(_filename);
        return NULL;
    }
    // Compile up the regular expressions
    int status = 0;
    int j = 0;

    status = regcomp(&regexComp, regPattern, REG_EXTENDED);
    if (status) {
        fprintf(stderr, "Unable to compile regular expression: %s\n", regPattern);
        return NULL;
    }


    char *buffer = NULL;
    size_t len = 0;
    ssize_t read;
    OBJECT_LIST_STR *list = NULL;
    char *ptr = NULL;
    OBJECT_STR *objPtr = NULL;
    char result[MAX_BUFFER_LEN + 1];
    int resultLen = 0;
    unsigned int offset = 0;
    char errorBuff[BUFSIZ + 1];
    while ((read = getline(&buffer, &len, fp)) != -1) {
        ptr = strchr(buffer, '\n');
        if (ptr != NULL) {
            *ptr = 0;
        }
        // First token in the object Name
        ptr = strchr(buffer, ',');
        if (ptr == NULL) {
            continue;
        }
        *ptr++ = 0; // Change the , to a zero and bump the pointer up one byte
        // Only allocate the list once - but only when we have a valid entry
        if (list == NULL) {
            list = calloc(sizeof (OBJECT_LIST_STR), 1);
            assert(list);
        }
        // Create a new object entry
        list->listOfObjects = (OBJECT_STR *) realloc(list->listOfObjects, sizeof (OBJECT_STR) * (1 + list->numObjects + 1));
        assert(list->listOfObjects);
        objPtr = (OBJECT_STR *) memset(&list->listOfObjects[list->numObjects++], 0, sizeof (OBJECT_STR));
        // Write in the object
        strcpy(objPtr->objectName, buffer);
        // Now start processing the properties
        // Start with the tuples
        char *next = ptr;


        for (j = 0; j < MAX_MATCH; j++) {
            status = regexec(&regexComp, next, 1, &match, 0);

            if (!status) {
                // We have a bunch of tuple patterns (properties)
                if (match.rm_so != -1) {
                    resultLen = match.rm_eo - match.rm_so;
                    memcpy(result, next + match.rm_so, resultLen);
                    result[resultLen] = 0;
                    offset = match.rm_eo;
                    // We now  have the buffer
                    objPtr = getPropertyValues(objPtr, result);

                }

            } else {
                if (status != REG_NOMATCH) {
                    regerror(status, &regexComp, errorBuff, BUFSIZ);
                    printf("regcomp: %s\n", errorBuff);
                }
                break;
            }
            next += offset;

        }
    }

    regfree(&regexComp);

    // Free the buffer allocated for reading the lines
    free(buffer);
    // Close the file pointer
    fclose(fp);

    return list;


}

void cleanUpObjectList(OBJECT_LIST_STR * _objectList) {

    if (_objectList == NULL) {
        return;
    }

    OBJECT_STR *obj = NULL;
    PROPERTY_STR *prop = NULL;
    for (int i = _objectList->numObjects - 1; i >= 0; i--) {
        obj = &_objectList->listOfObjects[i];
        for (int j = obj->numProperties - 1; j >= 0; j--) {
            prop = &obj->properties[j];
            free(prop->data);
        }
       
    }
    free(_objectList);
    _objectList = NULL;

}

OBJECT_STR * getPropertyValues(OBJECT_STR *_object, char *_result) {

    // So we have a tuple property 
    char *ptr = strchr(_result, ':');
    if (ptr == NULL) {
        return _object;
    }
    // Zero out and advance
    *ptr++ = 0;
    _object->properties = (PROPERTY_STR *) realloc(_object->properties, sizeof (PROPERTY_STR) * (1 + _object->numProperties + 1));
    // Check that it is not NULL
    assert(_object->properties);
    // Initiate  to zero's
    PROPERTY_STR *prop = (PROPERTY_STR *) memset(&_object->properties[_object->numProperties++], 0, sizeof (PROPERTY_STR));
    // Copy over the property name
    strcpy(prop->property, _result);
    // Now parse up  the number from what should be left - I.e. [1.0, 1,0] 
    char *delim = "[], ";
    char *num = strtok(ptr, delim); // parse up  by the brackets  any spaces and or commas
    double value = 0.0;
    while (num != NULL) {
        if (isdigit(num[0]) || (num[0] == '-' && isdigit(num[1]))) {
            value = atof(num);
            prop->data = (double *) realloc(prop->data, sizeof (double) * (1 + prop->numValues));
            assert(prop->data);
            prop->data[prop->numValues++] = value;
        }
        num = strtok(NULL, delim);
    }

    return _object;



}


