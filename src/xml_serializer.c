#include "xml_serializer.h"

void init_serializer(FILE *out){
	open_tag("?xml version=\"1.0\" encoding=\"UTF-8\"?", out, 0);
	putc('\n', out);
}

void write_tag(char *tag, char *content, FILE *out, int indent){
	open_tag(tag, out, indent);
	fprintf(out, "%s", content);
	close_tag(tag, out, indent);
	putc('\n', out);
}

void open_tag(char *tag, FILE *out, int indent){
	for (int i = 0; i < indent; ++i) {
		putc('\t', out);
	}
	fprintf(out, "<%s>", tag);
}

void close_tag(char *tag, FILE *out, int indent){
	for (int i = 0; i < indent; ++i) {
		putc('\t', out);
	}
	fprintf(out, "</%s>", tag);
}
