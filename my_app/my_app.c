#include <furi.h>
#include <gui/gui.h>
#include <gui/view_dispatcher.h>
#include <gui/modules/submenu.h>
#include <gui/modules/popup.h>
#include <input/input.h>

typedef enum {
    MyAppViewSubmenu,
    MyAppViewPopup,
} MyAppView;

typedef enum {
    MyAppItemHello,
    MyAppItemAbout,
} MyAppItem;

typedef struct {
    Gui*            gui;
    ViewDispatcher* view_dispatcher;
    Submenu*        submenu;
    Popup*          popup;
} MyApp;

static void my_app_menu_cb(void* ctx, uint32_t index) {
    MyApp* app = ctx;
    switch(index) {
    case MyAppItemHello:
        popup_set_header(app->popup, "Hello!", 64, 10, AlignCenter, AlignTop);
        popup_set_text(app->popup, "Template is alive.\nPress Back.", 64, 32, AlignCenter, AlignTop);
        view_dispatcher_switch_to_view(app->view_dispatcher, MyAppViewPopup);
        break;
    case MyAppItemAbout:
        popup_set_header(app->popup, "About", 64, 10, AlignCenter, AlignTop);
        popup_set_text(app->popup, "Flipper App Template\nv1.0", 64, 32, AlignCenter, AlignTop);
        view_dispatcher_switch_to_view(app->view_dispatcher, MyAppViewPopup);
        break;
    default:
        break;
    }
}

static uint32_t my_app_back_cb(void* ctx) {
    MyApp* app = ctx;
    view_dispatcher_switch_to_view(app->view_dispatcher, MyAppViewSubmenu);
    return VIEW_IGNORE;
}

int32_t my_app_main(void* p) {
    UNUSED(p);
    MyApp* app = malloc(sizeof(MyApp));
    memset(app, 0, sizeof(MyApp));

    app->gui = furi_record_open(RECORD_GUI);
    app->view_dispatcher = view_dispatcher_alloc();

    app->submenu = submenu_alloc();
    submenu_add_item(app->submenu, "Say Hello", MyAppItemHello, my_app_menu_cb, app);
    submenu_add_item(app->submenu, "About",     MyAppItemAbout, my_app_menu_cb, app);

    app->popup = popup_alloc();

    view_dispatcher_set_event_callback_context(app->view_dispatcher, app);
    view_dispatcher_set_navigation_event_callback(app->view_dispatcher, my_app_back_cb);

    view_dispatcher_add_view(app->view_dispatcher, MyAppViewSubmenu, submenu_get_view(app->submenu));
    view_dispatcher_add_view(app->view_dispatcher, MyAppViewPopup,   popup_get_view(app->popup));

    view_dispatcher_switch_to_view(app->view_dispatcher, MyAppViewSubmenu);
    view_dispatcher_attach_to_gui(app->view_dispatcher, app->gui, ViewDispatcherTypeFullscreen);
    view_dispatcher_run(app->view_dispatcher);

    view_dispatcher_remove_view(app->view_dispatcher, MyAppViewSubmenu);
    view_dispatcher_remove_view(app->view_dispatcher, MyAppViewPopup);
    submenu_free(app->submenu);
    popup_free(app->popup);
    view_dispatcher_free(app->view_dispatcher);
    furi_record_close(RECORD_GUI);
    free(app);
    return 0;
}
