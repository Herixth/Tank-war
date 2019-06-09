/**
* @File  menu.cpp
* @Brief 用来进行menu.h中各个类函数的具体实现
* @Date  2018/05
*/

#include "menu.h"

/********************** class: TTF_Render **********************/
Menu_Render::Menu_Render() {
    window = SDL_CreateWindow
    ("Tank War", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        window_size_x, window_size_y, SDL_WINDOW_SHOWN);
    rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
    SDL_RenderClear(rend);
}

Menu_Render::~Menu_Render() {
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(window);
}

SDL_Window *Menu_Render::GetWindow() {
    return this -> window;
}

SDL_Renderer *Menu_Render::GetRender() {
    return this -> rend;
}

/********************** class: Menu_SurTex **********************/
Menu_SurTex::Menu_SurTex(SDL_Renderer *rend) {
    MenuSur_list.push_back(IMG_Load("image/menu/background.png"));
    MenuTex_list.push_back(SDL_CreateTextureFromSurface(rend, MenuSur_list.back()));
    MenuSur_list.push_back(IMG_Load("image/menu/edge.png"));
    MenuTex_list.push_back(SDL_CreateTextureFromSurface(rend, MenuSur_list.back()));
    MenuSur_list.push_back(IMG_Load("image/menu/title.png"));
    MenuTex_list.push_back(SDL_CreateTextureFromSurface(rend, MenuSur_list.back()));
    MenuSur_list.push_back(IMG_Load("image/menu/direction.png"));
    MenuTex_list.push_back(SDL_CreateTextureFromSurface(rend, MenuSur_list.back()));
    MenuSur_list.push_back(IMG_Load("image/menu/pra_mod.png"));
    MenuTex_list.push_back(SDL_CreateTextureFromSurface(rend, MenuSur_list.back()));
    MenuSur_list.push_back(IMG_Load("image/menu/adv_mod.png"));
    MenuTex_list.push_back(SDL_CreateTextureFromSurface(rend, MenuSur_list.back()));
    MenuSur_list.push_back(IMG_Load("image/menu/circle.png"));
    MenuTex_list.push_back(SDL_CreateTextureFromSurface(rend, MenuSur_list.back()));
    MenuSur_list.push_back(IMG_Load("image/menu/backstep.png"));
    MenuTex_list.push_back(SDL_CreateTextureFromSurface(rend, MenuSur_list.back()));
    MenuSur_list.push_back(IMG_Load("image/menu/enter.png"));
    MenuTex_list.push_back(SDL_CreateTextureFromSurface(rend, MenuSur_list.back()));
    MenuSur_list.push_back(IMG_Load("image/menu/Mouse_point.png"));
 
    MenuTex_list.push_back(SDL_CreateTextureFromSurface(rend, MenuSur_list.back()));
    SDL_SetTextureBlendMode(*(MenuTex_list.begin() + _MENU_MOUSE_), SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(*(MenuTex_list.begin() + _MENU_MOUSE_), _ALPHA_TOTAL_);

    SDL_Rect tmp_rect;
    tmp_rect.x = tmp_rect.y = 0;
    tmp_rect.w = window_size_x;
    tmp_rect.h = window_size_y;
    MenuRect_list.push_back(tmp_rect);
    MenuRect_list.push_back(tmp_rect);
    tmp_rect.x = title_x; tmp_rect.y = title_y;        //TITLE
    tmp_rect.h = title_h; tmp_rect.w = title_w;
    MenuRect_list.push_back(tmp_rect);
    tmp_rect.x = conmon_x; tmp_rect.y = direct_y;      //DIRECT
    tmp_rect.h = conmon_h; tmp_rect.w = conmon_w;
    MenuRect_list.push_back(tmp_rect);
    tmp_rect.y = pra_mod_y;      //PRA_MOD
    MenuRect_list.push_back(tmp_rect);
    tmp_rect.y = adv_mod_y;      //ADV_MOD
    MenuRect_list.push_back(tmp_rect);
    tmp_rect.x = init_o_x; tmp_rect.y = init_o_y;      //CIRCLE
    tmp_rect.h = init_o_h; tmp_rect.w = init_o_w;
    MenuRect_list.push_back(tmp_rect);
    tmp_rect.x = back_x; tmp_rect.y = back_y;          //BACK
    tmp_rect.h = back_h; tmp_rect.w = back_w;
    MenuRect_list.push_back(tmp_rect);
    tmp_rect.y = enter_y;
    MenuRect_list.push_back(tmp_rect);
    tmp_rect.w = mouse_w; tmp_rect.h = mouse_h;
    MenuRect_list.push_back(tmp_rect);
    option = _OP_NULL_;
    con_op = _OP_NULL_;

}

Menu_SurTex::~Menu_SurTex() {
    for (unsigned int inc = _MENU_BCG_; inc < MenuSur_list.size(); inc ++) {
        SDL_FreeSurface(*(MenuSur_list.begin() + inc));
        SDL_DestroyTexture(*(MenuTex_list.begin() + inc));
    }
}

void Menu_SurTex::SetMotion_OP(SDL_Event &event) {
    int y_list[] = { direct_y, pra_mod_y, adv_mod_y };
    int new_op = _OP_DIRECT_ * _IS_DIRECT_ + _OP_PRA_MOD_ * _IS_PRA_MOD_ + _OP_ADV_MOD_ * _IS_ADV_MOD_;
    if (new_op && option != new_op) {
        if (option && option != con_op) {
            (*(MenuRect_list.begin() + _MENU_DIRECT_ - 1 + option)).y = y_list[option - 1];
        }
        if (con_op != new_op) {
            (*(MenuRect_list.begin() + _MENU_DIRECT_ - 1 + new_op)).y -= motion_wave;
        }
        option = new_op;
    }
    else if (!new_op && option && option != con_op) {
        (*(MenuRect_list.begin() + _MENU_DIRECT_ - 1 + option)).y = y_list[option - 1];
        option = new_op;
    }
    else if (!new_op) {
        option = new_op;
    }
}

bool Menu_SurTex::SetMouseDown_OP(SDL_Event &event, GameMode &gamemode) {
    int y_list[] = { direct_y, pra_mod_y, adv_mod_y };
    if (option == con_op && con_op) {
        gamemode.SetMode(con_op);
        return true;
    }
    else if (option && option != con_op) {
        (*(MenuRect_list.begin() + _MENU_DIRECT_ - 1 + option)).y = y_list[option - 1];
        con_op = option;
        return false;
    }
    else if (!option) {
        con_op = option;
        return false;
    }
    return false;
}

bool Menu_SurTex::SetKeyDown_OP(SDL_Event &event, GameMode &gamemode) {
    int y_list[] = { direct_y, pra_mod_y, adv_mod_y };
    switch (event.key.keysym.sym) {
    case SDLK_w: case SDLK_UP:
        if (con_op > _OP_DIRECT_) {
            if (-- con_op == option) {
                (*(MenuRect_list.begin() + _MENU_DIRECT_ - 1 + option)).y = y_list[option - 1];
            }
        }
        break;
    case SDLK_s: case SDLK_DOWN:
        if (con_op < _OP_ADV_MOD_) {
            if (++ con_op == option) {
                (*(MenuRect_list.begin() + _MENU_DIRECT_ - 1 + option)).y = y_list[option - 1];
            }
        }
        break;
    case SDLK_j: case SDLK_SPACE:
        if (!con_op) {
            return false;
        }
        gamemode.SetMode(con_op);
        return true;
        break;
    default:
        break;
    }
    return false;
}

bool Menu_SurTex::DrawStep(SDL_Renderer *render, SDL_Event &event, int option_num) {
    int IS_list[] = { _IS_BACK_, _IS_ENTER_ };
    int y_list[] = { back_y, enter_y };
    switch (event.type) {
    case SDL_QUIT:
        SDL_Quit();
        exit(1);
        break;
    case SDL_MOUSEMOTION:
        (*(MenuRect_list.begin() + option_num)).y +=
            IS_list[option_num - _MENU_BACK_] ? ((*(MenuRect_list.begin() + option_num)).y == y_list[option_num - _MENU_BACK_]) * (-motion_wave) : ((*(MenuRect_list.begin() + option_num)).y != y_list[option_num - _MENU_BACK_]) * motion_wave;
        SDL_RenderCopy(render, (*(MenuTex_list.begin() + option_num)), NULL, &(*(MenuRect_list.begin() + option_num)));
        return false;
        break;
    case SDL_MOUSEBUTTONDOWN:
        if (IS_list[option_num - _MENU_BACK_]) {
            (*(MenuRect_list.begin() + option_num)).y = y_list[option_num - _MENU_BACK_];
            SDL_RenderCopy(render, (*(MenuTex_list.begin() + option_num)), NULL, &(*(MenuRect_list.begin() + option_num)));
            return true;
        }
        SDL_RenderCopy(render, (*(MenuTex_list.begin() + option_num)), NULL, &(*(MenuRect_list.begin() + option_num)));
        break;
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE && option_num == _MENU_BACK_) {
            SDL_RenderCopy(render, (*(MenuTex_list.begin() + option_num)), NULL, &(*(MenuRect_list.begin() + option_num)));
            return true;
        }
        if((event.key.keysym.sym == SDLK_SPACE || event.key.keysym.sym == SDLK_j) && option_num == _MENU_ENTER_) {
            SDL_RenderCopy(render, (*(MenuTex_list.begin() + option_num)), NULL, &(*(MenuRect_list.begin() + option_num)));
            return true;
        }
        SDL_RenderCopy(render, (*(MenuTex_list.begin() + option_num)), NULL, &(*(MenuRect_list.begin() + option_num)));
        break;
    default:
        SDL_RenderCopy(render, (*(MenuTex_list.begin() + option_num)), NULL, &(*(MenuRect_list.begin() + option_num)));
        return false;
        break;
    }
    return false;
}

void Menu_SurTex::ResetMouseRect(SDL_Event &event) {
    (*(MenuRect_list.begin() + _MENU_MOUSE_)).x = event.button.x;
    (*(MenuRect_list.begin() + _MENU_MOUSE_)).y = event.button.y;
}

void Menu_SurTex::DrawCircle(SDL_Renderer *render, int op) {
    (*(MenuRect_list.begin() + _MENU_CIRCLE_)).y = init_o_y + (*(MenuRect_list.begin() + _MENU_DIRECT_ - 1 + op)).y - direct_y;
    SDL_RenderCopy(render, (*(MenuTex_list.begin() + _MENU_CIRCLE_)), NULL, &(*(MenuRect_list.begin() + _MENU_CIRCLE_)));
}

bool Menu_SurTex::CmpCon_op(int op) {
    return (op == con_op);
}

SDL_Surface *Menu_SurTex::GetSurface(int order) {
    return *(MenuSur_list.begin() + order);
}

SDL_Texture *Menu_SurTex::GetTexture(int order) {
    return *(MenuTex_list.begin() + order);
}

SDL_Rect Menu_SurTex::GetRect(int order) {
    return *(MenuRect_list.begin() + order);
}


/*          非类函数           */

void DrawMenu(Menu_Render &menu_render, Menu_SurTex &menu_surtex) {
    SDL_RenderCopy(menu_render.GetRender(), menu_surtex.GetTexture(_MENU_BCG_), NULL, &menu_surtex.GetRect(_MENU_BCG_));
    SDL_RenderCopy(menu_render.GetRender(), menu_surtex.GetTexture(_MENU_EDGE_), NULL, &menu_surtex.GetRect(_MENU_EDGE_));
    SDL_RenderCopy(menu_render.GetRender(), menu_surtex.GetTexture(_MENU_TITLE_), NULL, &menu_surtex.GetRect(_MENU_TITLE_));
    SDL_RenderCopy(menu_render.GetRender(), menu_surtex.GetTexture(_MENU_MOUSE_), NULL, &menu_surtex.GetRect(_MENU_MOUSE_));

    int _OP_list[] = { _OP_DIRECT_, _OP_PRA_MOD_, _OP_ADV_MOD_ };
    for (int inc = 0; inc < 3; inc ++) {
        if (menu_surtex.CmpCon_op(_OP_list[inc])) {
            menu_surtex.DrawCircle(menu_render.GetRender(), _OP_list[inc]);
        }
        SDL_RenderCopy(menu_render.GetRender(), menu_surtex.GetTexture(_MENU_DIRECT_ + inc), NULL, &menu_surtex.GetRect(_MENU_DIRECT_ + inc));
    }
}

void DirectMod(SDL_Renderer *render, Menu_SurTex &menu_surtex) {
    SDL_Event event;
    bool quit = false;
    int num = 0;
    while (!quit) {
        if (SDL_WaitEvent(&event)) {
            SDL_RenderClear(render);
            SDL_RenderCopy(render, menu_surtex.GetTexture(_MENU_BCG_), NULL, &menu_surtex.GetRect(_MENU_BCG_));
            SDL_RenderCopy(render, menu_surtex.GetTexture(_MENU_EDGE_), NULL, &menu_surtex.GetRect(_MENU_EDGE_));
            quit = menu_surtex.DrawStep(render, event, _MENU_BACK_);
            menu_surtex.ResetMouseRect(event);
            SDL_RenderCopy(render, menu_surtex.GetTexture(_MENU_MOUSE_), NULL, &menu_surtex.GetRect(_MENU_MOUSE_));
            SDL_RenderPresent(render);
            SDL_Delay((40 * quit + 1) * delay_menu_time);
        }
    }
}

bool Pra_Mod(GameMode &gamemode, SDL_Renderer *render, Menu_SurTex &menu_surtex) {
    SDL_Event event;
    bool quit = false;
    bool enter = false;
    while (!quit && !enter) {
        if (SDL_WaitEvent(&event)) {
            SDL_RenderClear(render);
            SDL_RenderCopy(render, menu_surtex.GetTexture(_MENU_BCG_), NULL, &menu_surtex.GetRect(_MENU_BCG_));
            SDL_RenderCopy(render, menu_surtex.GetTexture(_MENU_EDGE_), NULL, &menu_surtex.GetRect(_MENU_EDGE_));
            quit = menu_surtex.DrawStep(render, event, _MENU_BACK_);
            enter = menu_surtex.DrawStep(render, event, _MENU_ENTER_);
            menu_surtex.ResetMouseRect(event);
            SDL_RenderCopy(render, menu_surtex.GetTexture(_MENU_MOUSE_), NULL, &menu_surtex.GetRect(_MENU_MOUSE_));
            SDL_RenderPresent(render);
            SDL_Delay((40 * (quit + enter) + 1) * delay_menu_time);
        }
    }
    return (!quit || enter);
}

bool Adv_Mod(GameMode &gamemode, SDL_Renderer *render, Menu_SurTex &menu_surtex) {
    SDL_Event event;
    bool quit = false;
    bool enter = false;
    while (!quit && !enter) {
        if (SDL_WaitEvent(&event)) {
            SDL_RenderClear(render);
            SDL_RenderCopy(render, menu_surtex.GetTexture(_MENU_BCG_), NULL, &menu_surtex.GetRect(_MENU_BCG_));
            SDL_RenderCopy(render, menu_surtex.GetTexture(_MENU_EDGE_), NULL, &menu_surtex.GetRect(_MENU_EDGE_));
            quit = menu_surtex.DrawStep(render, event, _MENU_BACK_);
            enter = menu_surtex.DrawStep(render, event, _MENU_ENTER_);
            menu_surtex.ResetMouseRect(event);
            SDL_RenderCopy(render, menu_surtex.GetTexture(_MENU_MOUSE_), NULL, &menu_surtex.GetRect(_MENU_MOUSE_));
            SDL_RenderPresent(render);
            SDL_Delay((40 * (quit + enter) + 1) * delay_menu_time);
        }
    }
    return (!quit || enter);
}


bool GamemodeCal(GameMode &gamemode, Menu_Render &menu_render, Menu_SurTex &menu_surtex) {
    switch (gamemode.GetMode()) {
    case _OP_DIRECT_:
        DirectMod(menu_render.GetRender(), menu_surtex);
        return false;
        break;
    case _OP_PRA_MOD_:
        return Pra_Mod(gamemode, menu_render.GetRender(), menu_surtex);
        break;
    case _OP_ADV_MOD_:
        return Adv_Mod(gamemode, menu_render.GetRender(), menu_surtex);
        break;
    default:
        break;
    }
    return false;
}

GameMode MenuWorking() {
    /*     隐藏鼠标指针    */
    SDL_ShowCursor(false);

    GameMode gamemode;
    Menu_Render menu_render;
    Menu_SurTex menu_surtex(menu_render.GetRender());

    SDL_Event event;
    bool quit = false;
    while (!quit) {
        if (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                quit = true;
                SDL_Quit();
                exit(1);
                break;
            case SDL_MOUSEMOTION:
                menu_surtex.SetMotion_OP(event);
                menu_surtex.ResetMouseRect(event);
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (menu_surtex.SetMouseDown_OP(event, gamemode)) {
                    quit = GamemodeCal(gamemode, menu_render, menu_surtex);
                    menu_surtex.ResetOption();
                }
                break;
            case SDL_KEYDOWN:
                if (menu_surtex.SetKeyDown_OP(event, gamemode)) {
                    quit = GamemodeCal(gamemode, menu_render, menu_surtex);
                    menu_surtex.ResetOption();
                }
                break;
            default:
                break;
            }
        }
        if (!quit) {
            SDL_RenderClear(menu_render.GetRender());
            DrawMenu(menu_render, menu_surtex);
            SDL_RenderPresent(menu_render.GetRender());
            SDL_Delay(delay_menu_time);
        }
    }
    
    return gamemode;
}


