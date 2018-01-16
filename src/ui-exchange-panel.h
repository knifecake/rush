#ifndef __UI_EXCHANGE_PANEL__
#define __UI_EXCHANGE_PANEL__

#include "entities/world.h"

/*
 * UIExchangePanel
 *
 * A Panel to exchange resources.
 */
typedef struct _UIExchangePanel UIExchangePanel;

/*
 * Creates a new panel to display different exchangeings to be built.
 */
UIExchangePanel *ui_exchange_panel_new(World *w);

/*
 * Controls the exchange panel, returns price selected.
 */
int ui_exchange_panel_control(UIExchangePanel *exchangepanel, int *resource_id);

/*
 * Frees all memory allocated by ui_map_new.
 */
void ui_exchange_panel_destroy(UIExchangePanel *ep);

#endif
