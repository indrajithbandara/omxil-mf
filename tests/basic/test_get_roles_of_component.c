
#include <stdio.h>
#include <stdlib.h>

#include <OMX_Core.h>

#include "common/test_omxil.h"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))


int main(int argc, char *argv[])
{
	const char *arg_comp;
	char name_comp[OMX_MAX_STRINGNAME_SIZE];
	OMX_U32 num_roles;
	OMX_U8 *name_roles[128] = {NULL, };
	OMX_ERRORTYPE result;
	OMX_U32 i;
	
	//get arguments
	if (argc < 2) {
		arg_comp = "OMX.st.audio_decoder.mp3";
	} else {
		arg_comp = argv[1];
	}
	
	result = OMX_ErrorNone;
	
	result = OMX_Init();
	if (result != OMX_ErrorNone) {
		fprintf(stderr, "OMX_Init failed.\n");
		goto err_out1;
	}
	
	snprintf(name_comp, sizeof(name_comp), arg_comp);
	num_roles = ARRAY_SIZE(name_roles);
	for (i = 0; i < ARRAY_SIZE(name_roles); i++) {
		name_roles[i] = (OMX_U8 *)malloc(OMX_MAX_STRINGNAME_SIZE);
		if (name_roles[i] == NULL) {
			goto err_out3;
		}
	}
	result = OMX_GetRolesOfComponent(name_comp, &num_roles, name_roles);
	if (result != OMX_ErrorNone) {
		fprintf(stderr, "OMX_GetRolesOfComponent failed.\n");
		goto err_out3;
	}
	
	printf("component:'%s', roles:%d\n", name_comp, num_roles);
	for (i = 0; i < num_roles; i++) {
		printf("%2d: role:'%s'\n", i, name_roles[i]);
	}
	
	
	for (i = 0; i < ARRAY_SIZE(name_roles); i++) {
		free(name_roles[i]);
		name_roles[i] = NULL;
	}
	
	result = OMX_Deinit();
	if (result != OMX_ErrorNone) {
		fprintf(stderr, "OMX_Deinit failed.\n");
		goto err_out1;
	}
	
	return 0;
	
err_out3:
	for (i = 0; i < ARRAY_SIZE(name_roles); i++) {
		free(name_roles[i]);
		name_roles[i] = NULL;
	}
	
//err_out2:
	OMX_Deinit();
	
err_out1:
	return result;
}