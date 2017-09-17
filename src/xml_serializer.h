#ifndef XML_SERIALIZER_H
#define XML_SERIALIZER_H

#include <stdio.h>

/*
 * Write XML opening tag to file.
 * @param out 	 write target file
 */
void init_serializer(FILE *out);

/*
 * Write given tag with given content to file.
 * @param tag     	 name of tag
 * @param content 	 tag contents
 * @param out     	 write target file
 * @param indent  	 indent level
 */
void write_tag(char *tag, char *content, FILE *out, int indent);

/*
 * Write opening tag to target file.
 * @param tag   	 tag name
 * @param out    	 write target file
 * @param indent 	 indent level
 */
void open_tag(char *tag, FILE *out, int indent);

/*
 * Write closing tag to target file.
 * @param tag   	 tag name
 * @param out    	 write target file
 * @param indent 	 indent level
 */
void close_tag(char *tag, FILE *out, int indent);

#endif /* ifndef XML_SERIALIZER_H */
