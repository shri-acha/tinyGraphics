#include "graphics.h"
#include "_graphics.h"
#include "types.h"
#include <stdio.h>

int loadRenderedObjectToContext(renderContext rc, renderedObject* ro){
	if ((*rc.scene_context)->obj != NULL) {
		if ((*rc.scene_context)->next != NULL ) {
			fprintf(stderr,"[ERROR] failed load object");
		} else {

		}
	}
}
sceneContext newSceneContext(sceneContext* sc,renderedObject* ro) {

	if (sc->obj == NULL) {
		return (sceneContext) {
			.obj = ro,
			.next = NULL,
			.prev = NULL,
		};
	}
	else if (sc->obj != NULL && sc->next == NULL) {}

}
