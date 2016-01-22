//#include "../constants.h"
#include "../common.h"
#include "client_func.h"

static SDL_Event event;

/*****************************************************************
void EventTitle(int myid, int sock)
引数：
機能：
返値：
*****************************************************************/
void EventTitle(int myid, int sock){
    CONTAINER data;
    memset(&data, 0, sizeof(CONTAINER));

    //SDL_Event event;

//コントローラーから
    if(SDL_PollEvent(&event)){
        switch(event.type){

        case SDL_QUIT:
            data.command = END_COMMAND;
            fprintf(stderr, "%d\n", data.command);
            data.cid = myid;
            data.state = GAME_TITLE;
            send_data(&data, sizeof(CONTAINER), sock);
            break;

        case SDL_KEYDOWN:
            switch(event.key.keysym.sym){
            case SDLK_ESCAPE:
                data.command = END_COMMAND;
                data.cid = myid;
                data.state = GAME_TITLE;
                send_data(&data, sizeof(CONTAINER), sock);
                break;
            default:
                fprintf(stderr, "not a valid command.\n");
                break;
            }
            break;

            // ジョイスティックのボタンが押された時
        case SDL_JOYBUTTONDOWN:
            printf("The ID of the pressed button is %d.\n", event.jbutton.button);	// 押されたボタンのIDを表示（0から）
            // ボタンIDに応じた処理
            switch(event.jbutton.button){
            case 3:
                fprintf(stderr, "4 push.\n");
                data.command = FOUR_COMMAND;
                data.cid = myid;
                data.state = GAME_TITLE;
                send_data(&data, sizeof(CONTAINER), sock);
                stageFlag = 2;
                break;
            }
            break;
        }
    }
}


/*****************************************************************
void EventSelect(int myid, int sock)
引数：
機能：
返値：
*****************************************************************/
void EventSelect(int myid, int sock){
    CONTAINER data;
    memset(&data, 0, sizeof(CONTAINER));

    //SDL_Event event;

//コントローラーから
    if(SDL_PollEvent(&event)){
        switch(event.type){

        case SDL_QUIT:
            data.command = END_COMMAND;
            fprintf(stderr, "%d\n", data.command);
            data.cid = myid;
            data.state = GAME_SELECT;
            send_data(&data, sizeof(CONTAINER), sock);
            break;

        case SDL_KEYDOWN:
            switch(event.key.keysym.sym){
            case SDLK_ESCAPE:
                data.command = END_COMMAND;
                data.cid = myid;
                data.state = GAME_SELECT;
                send_data(&data, sizeof(CONTAINER), sock);
                break;
            default:
                fprintf(stderr, "not a valid command.\n");
                break;
            }
            break;


	case SDL_JOYAXISMOTION:
            //printf("The axis ID of the operated key is %d.\n",event.jaxis.axis);	// 操作された方向キーの方向軸を表示（0：アナログキー，1：アナログキー，2：方向キー左右方向，3：方向キー上下方向）
            if(stageFlag == 1){ //決定していないとき
                if(event.jaxis.axis==0){
                    if(event.jaxis.value == -32768){
                        data.command = LEFT_COMMAND;
                        data.cid = myid;
                        data.state = GAME_SELECT;
                        send_data(&data, sizeof(CONTAINER), sock);
                        //printf("左\n");
                    }
                    if(event.jaxis.value == 32767){
                        data.command = RIGHT_COMMAND;
                        data.cid = myid;
                        data.state = GAME_SELECT;
                        send_data(&data, sizeof(CONTAINER), sock);
                        //printf("右\n");
                    }
                    //printf("--- Analog-Direction Key: ?? Axis\n");
                }
                else if(event.jaxis.axis==1){
                    if(event.jaxis.value == -32768){
                        data.command = UP_COMMAND;
                        data.cid = myid;
                        data.state = GAME_SELECT;
                        send_data(&data, sizeof(CONTAINER), sock);
                        //printf("上\n");
                    }
                    if(event.jaxis.value == 32767){
                        data.command = DOWN_COMMAND;
                        data.cid = myid;
                        data.state = GAME_SELECT;
                        send_data(&data, sizeof(CONTAINER), sock);
                        //printf("下\n");
                    }
                    //printf("--- Anolag-Direction Key: ?? Axis\n");
                }
            }
            break;

            // ジョイスティックのボタンが押された時
        case SDL_JOYBUTTONDOWN:
            printf("The ID of the pressed button is %d.\n", event.jbutton.button);	// 押されたボタンのIDを表示（0から）
            // ボタンIDに応じた処理
            switch(event.jbutton.button){
//弾を打つ
            case 3:
                //if(pla_sele[myid].kPflag == 0) {
                    fprintf(stderr, "4 push.\n");
                    data.command = FOUR_COMMAND;
                    data.cid = myid;
                    data.state = GAME_SELECT;
                    data.kndP = pla_sele[myid].kndP; //決定
                    send_data(&data, sizeof(CONTAINER), sock);
                    stageFlag = 2;
                    //}
                break;
            }
            break;
        }
    }
}


/*****************************************************************
void EventLoad(int myid, int sock)
引数：
機能：
返値：
*****************************************************************/
void EventLoad(int myid, int sock){
    CONTAINER data;
    memset(&data, 0, sizeof(CONTAINER));

    //SDL_Event event;

//コントローラーから
    if(SDL_PollEvent(&event)){
        switch(event.type){

        case SDL_QUIT:
            data.command = END_COMMAND;
            fprintf(stderr, "%d\n", data.command);
            data.cid = myid;
            data.state = GAME_LOAD;
            send_data(&data, sizeof(CONTAINER), sock);
            break;

        case SDL_KEYDOWN:
            switch(event.key.keysym.sym){
            case SDLK_ESCAPE:
                data.command = END_COMMAND;
                data.cid = myid;
                data.state = GAME_LOAD;
                send_data(&data, sizeof(CONTAINER), sock);
                break;
            default:
                fprintf(stderr, "not a valid command.\n");
                break;
            }
            break;
        }
    }
}


/*****************************************************************
´Ø¿ôÌ¾	: EventMainFighter
µ¡Ç½	: ¥á¥¤¥ó¥¦¥¤¥ó¥É¥¦¤ËÂÐ¤¹¤ë¥¤¥Ù¥ó¥È½èÍý¤ò¹Ô¤¦
°ú¿ô	: int		num		: Á´¥¯¥é¥¤¥¢¥ó¥È¿ô
½ÐÎÏ	: ¤Ê¤·
*****************************************************************/
void EventMainFighter(int myid, int sock)
{
    CONTAINER data;
    memset(&data, 0, sizeof(CONTAINER));

    //SDL_Event event;
    //int buttonNO;

    /* °ú¤­¿ô¥Á¥§¥Ã¥¯ */
    //assert(0<num && num<=MAX_CLIENTS);

//コントローラーから
    if(SDL_PollEvent(&event)){
        switch(event.type){

        case SDL_QUIT:
            data.command = END_COMMAND;
            fprintf(stderr, "%d\n", data.command);
            data.cid = myid;
            data.state = GAME_MAIN;
            send_data(&data, sizeof(CONTAINER), sock);
            break;


        case SDL_KEYDOWN:
            switch(event.key.keysym.sym){
            case SDLK_ESCAPE:
                data.command = END_COMMAND;
                data.cid = myid;
                data.state = GAME_MAIN;
                send_data(&data, sizeof(CONTAINER), sock);
                break;
            default:
                fprintf(stderr, "not a valid command.\n");
                break;
            }
            break;


	case SDL_JOYAXISMOTION:
            //printf("The axis ID of the operated key is %d.\n",event.jaxis.axis);	// 操作された方向キーの方向軸を表示（0：アナログキー，1：アナログキー，2：方向キー左右方向，3：方向キー上下方向）
            if(event.jaxis.axis==0){
                if(event.jaxis.value == -32768){
                    data.command = LEFT_COMMAND;
                    data.cid = myid;
                    data.state = GAME_MAIN;
                    send_data(&data, sizeof(CONTAINER), sock);
                    //printf("左\n");
                }
                if(event.jaxis.value == 32767){
                    data.command = RIGHT_COMMAND;
                    data.cid = myid;
                    data.state = GAME_MAIN;
                    send_data(&data, sizeof(CONTAINER), sock);
                    //printf("右\n");
                }
                //printf("--- Analog-Direction Key: ?? Axis\n");
            }
            else if(event.jaxis.axis==1){
                if(event.jaxis.value == -32768){
                    data.command = UP_COMMAND;
                    data.cid = myid;
                    data.state = GAME_MAIN;
                    send_data(&data, sizeof(CONTAINER), sock);
                    //printf("上\n");
                }
                if(event.jaxis.value == 32767){
                    data.command = DOWN_COMMAND;
                    data.cid = myid;
                    data.state = GAME_MAIN;
                    send_data(&data, sizeof(CONTAINER), sock);
                    //printf("下\n");
                }
                //printf("--- Anolag-Direction Key: ?? Axis\n");
            }					
            //printf("--- The axis value of the operated key is %d.\n",event.jaxis.value);	// ジョイスティックの操作された方向キーの値を表示（-32767（真左，真上）～32767（真右，真下））
            if(event.jaxis.value == 0){
                switch(event.jaxis.axis){
                case 0:
                    data.command = SEPARATE_LERI_COMMAND; //server.cのcontrol_requests関数より
                    data.cid = myid; //送信をしたIDの挿入
                    data.state = GAME_MAIN;
	            //data.tx = player[myid].tx;
                    //data.ty = player[myid].ty;
                    send_data(&data, sizeof(CONTAINER), sock); //dataの書き込み
                    //printf("%d : 左スティックを離した\n", event.jaxis.axis);
                    break;
                case 1:
                    data.command = SEPARATE_UPDO_COMMAND; //server.cのcontrol_requests関数より
                    data.cid = myid; //送信をしたIDの挿入
                    data.state = GAME_MAIN;
	            //data.tx = player[myid].tx;
                    //data.ty = player[myid].ty;
                    send_data(&data, sizeof(CONTAINER), sock); //dataの書き込み
                    break;
                }
            }
            break;


            // ジョイスティックのボタンが押された時
        case SDL_JOYBUTTONDOWN:
            //printf("The ID of the pressed button is %d.\n", event.jbutton.button);	// 押されたボタンのIDを表示（0から）
            // ボタンIDに応じた処理
            switch(event.jbutton.button){
//弾を打つ
            case 5:
                data.command = SHOT_COMMAND;
                data.cid = myid;
                data.state = GAME_MAIN;
                send_data(&data, sizeof(CONTAINER), sock);
                break;
            }

            if(event.jbutton.button==0){
                //printf("--- You pressed a button on the joystick.\n");
            }
            break;

            // ボタンが離された時
        case SDL_JOYBUTTONUP:
            //printf("The ID of the released button is %d.\n", event.jbutton.button);	// 離されたボタンのIDを表示（0から）
            // ボタンIDに応じた処理
            switch(event.jbutton.button){
            case 5:
                data.command = SHOT_FINISH_COMMAND; //server.cのcontrol_requests関数より
                data.cid = myid; //送信をしたIDの挿入
                data.state = GAME_MAIN;
	        //data.tx = player[myid].tx;
                //data.ty = player[myid].ty;
                send_data(&data, sizeof(CONTAINER), sock); //dataの書き込み
                //SendShotFinishCommand(1);
                break;
            }

            if(event.jbutton.button==0){
                //printf("--- You released a button on the joystick.\n");
            }
            break;
        }
    }
}



/*****************************************************************
´Ø¿ôÌ¾	: EventMainTank
µ¡Ç½	: ¥á¥¤¥ó¥¦¥¤¥ó¥É¥¦¤ËÂÐ¤¹¤ë¥¤¥Ù¥ó¥È½èÍý¤ò¹Ô¤¦
°ú¿ô	: int		num		: Á´¥¯¥é¥¤¥¢¥ó¥È¿ô
½ÐÎÏ	: ¤Ê¤·
*****************************************************************/
void EventMainTank(int myid, int sock){
    CONTAINER data;
    memset(&data, 0, sizeof(CONTAINER));
    SDL_Event event;

//コントローラーから
    if(SDL_PollEvent(&event)){
        switch(event.type){

        case SDL_QUIT:
            data.command = END_COMMAND;
            data.cid = myid;
            data.state = GAME_MAIN;
            send_data(&data, sizeof(CONTAINER), sock);
            break;


        case SDL_KEYDOWN:
            switch(event.key.keysym.sym){
            case SDLK_ESCAPE:
                data.command = END_COMMAND;
                data.cid = myid;
                data.state = GAME_MAIN;
                send_data(&data, sizeof(CONTAINER), sock);
                break;
            default:
                fprintf(stderr, "not a valid command.\n");
                break;
            }
            break;


	case SDL_JOYAXISMOTION:
            //printf("The axis ID of the operated key is %d.\n",event.jaxis.axis);	// 操作された方向キーの方向軸を表示（0：アナログキー，1：アナログキー，2：方向キー左右方向，3：方向キー上下方向）
            if(event.jaxis.axis==0){
                if(event.jaxis.value == -32768){
                    //SendLeftCommand(1);
                    data.command = LEFT_COMMAND;
                    data.cid = myid;
                    data.state = GAME_MAIN;
                    send_data(&data, sizeof(CONTAINER), sock);
                    //printf("左\n");
                }
                if(event.jaxis.value == 32767){
                    //SendRightCommand(1);
                    data.command = RIGHT_COMMAND;
                    data.cid = myid;
                    data.state = GAME_MAIN;
                    send_data(&data, sizeof(CONTAINER), sock);
                    //printf("右\n");
                }
                //printf("--- Analog-Direction Key: ?? Axis\n");
            }					
            //printf("--- The axis value of the operated key is %d.\n",event.jaxis.value);	// ジョイスティックの操作された方向キーの値を表示（-32767（真左，真上）～32767（真右，真下））
            if(event.jaxis.value == 0){
                //SendSeparateCommand(1, event.jaxis.axis);
                data.command = SEPARATE_LERI_COMMAND; //server.cのcontrol_requests関数より
                    data.cid = myid; //送信をしたIDの挿入
                    data.state = GAME_MAIN;
                    send_data(&data, sizeof(CONTAINER), sock);
                //printf("%d : 左スティックを離した\n", event.jaxis.axis);
            }
            break;


            // ジョイスティックのボタンが押された時
        case SDL_JOYBUTTONDOWN:
            //printf("The ID of the pressed button is %d.\n", event.jbutton.button);	// 押されたボタンのIDを表示（0から）
            // ボタンIDに応じた処理
            switch(event.jbutton.button){
//砲台を動かす
            case 0:
                data.command = UP_ROTA;
                data.cid = myid;
                data.state = GAME_MAIN;
                send_data(&data, sizeof(CONTAINER), sock);
                break;
            case 1:
                data.command = RIGHT_ROTA;
                data.cid = myid;
                data.state = GAME_MAIN;
                send_data(&data, sizeof(CONTAINER), sock);
                break;
            case 3:
                data.command = LEFT_ROTA;
                data.cid = myid;
                data.state = GAME_MAIN;
                send_data(&data, sizeof(CONTAINER), sock);
                break;

//バリア
            case 4:
                data.command = BARRIER_COMMAND;
                data.cid = myid;
                data.state = GAME_MAIN;
                send_data(&data, sizeof(CONTAINER), sock);
                break;

//弾を打つ
            case 5:
                //SendShotCommand(1);
                data.command = SHOT_COMMAND;
                data.cid = myid;
                data.state = GAME_MAIN;
                send_data(&data, sizeof(CONTAINER), sock);
                break;
            }

            if(event.jbutton.button==0){
                //printf("--- You pressed a button on the joystick.\n");
            }
            break;

            // ボタンが離された時
        case SDL_JOYBUTTONUP:
            //printf("The ID of the released button is %d.\n", event.jbutton.button);	// 離されたボタンのIDを表示（0から）
            // ボタンIDに応じた処理
            switch(event.jbutton.button){
            case 0:
                data.command = UP_SEPA_ROTA;
                data.cid = myid;
                data.state = GAME_MAIN;
                send_data(&data, sizeof(CONTAINER), sock);
                break;
            case 1:
                data.command = RIGHT_SEPA_ROTA;
                data.cid = myid;
                data.state = GAME_MAIN;
                send_data(&data, sizeof(CONTAINER), sock);
                break;
            case 3:
                data.command = LEFT_SEPA_ROTA;
                data.cid = myid;
                data.state = GAME_MAIN;
                send_data(&data, sizeof(CONTAINER), sock);
                break;

            case 4:
                data.command = BARRIER_FINISH_COMMAND;
                data.cid = myid;
                data.state = GAME_MAIN;
                send_data(&data, sizeof(CONTAINER), sock);
                break;

            case 5:
                //SendShotFinishCommand(1);
                data.command = SHOT_FINISH_COMMAND; //server.cのcontrol_requests関数より
                data.cid = myid; //送信をしたIDの挿入
                data.state = GAME_MAIN;
                send_data(&data, sizeof(CONTAINER), sock); //dataの書き込み
                break;
            }

            if(event.jbutton.button==0){
                //printf("--- You released a button on the joystick.\n");
            }
            break;
        }
    }
}


/*****************************************************************
void EventOver(int myid, int sock)
引数：
機能：
返値：
*****************************************************************/
void EventOver(int myid, int sock){
    CONTAINER data;
    memset(&data, 0, sizeof(CONTAINER));

    //SDL_Event event;

//コントローラーから
    if(SDL_PollEvent(&event)){
        switch(event.type){

        case SDL_QUIT:
            data.command = END_COMMAND;
            fprintf(stderr, "%d\n", data.command);
            data.cid = myid;
            data.state = GAME_OVER;
            send_data(&data, sizeof(CONTAINER), sock);
            break;

        case SDL_KEYDOWN:
            switch(event.key.keysym.sym){
            case SDLK_ESCAPE:
                data.command = END_COMMAND;
                data.cid = myid;
                data.state = GAME_OVER;
                send_data(&data, sizeof(CONTAINER), sock);
                break;
            default:
                fprintf(stderr, "not a valid command.\n");
                break;
            }
            break;

            // ジョイスティックのボタンが押された時
        case SDL_JOYBUTTONDOWN:
            printf("The ID of the pressed button is %d.\n", event.jbutton.button);	// 押されたボタンのIDを表示（0から）
            // ボタンIDに応じた処理
            switch(event.jbutton.button){
//弾を打つ
            case 3:
                fprintf(stderr, "4 push.\n");
                data.command = FOUR_COMMAND;
                data.cid = myid;
                data.state = GAME_OVER;
                send_data(&data, sizeof(CONTAINER), sock);
                stageFlag = 2;
                break;
            }
            break;
        }
    }
}


/*****************************************************************
void EventClear(int myid, int sock)
引数：
機能：
返値：
*****************************************************************/
void EventClear(int myid, int sock){
    CONTAINER data;
    memset(&data, 0, sizeof(CONTAINER));

    //SDL_Event event;

//コントローラーから
    if(SDL_PollEvent(&event)){
        switch(event.type){

        case SDL_QUIT:
            data.command = END_COMMAND;
            fprintf(stderr, "%d\n", data.command);
            data.cid = myid;
            data.state = GAME_CLEAR;
            send_data(&data, sizeof(CONTAINER), sock);
            break;

        case SDL_KEYDOWN:
            switch(event.key.keysym.sym){
            case SDLK_ESCAPE:
                data.command = END_COMMAND;
                data.cid = myid;
                data.state = GAME_CLEAR;
                send_data(&data, sizeof(CONTAINER), sock);
                break;
            default:
                fprintf(stderr, "not a valid command.\n");
                break;
            }
            break;

            // ジョイスティックのボタンが押された時
        case SDL_JOYBUTTONDOWN:
            printf("The ID of the pressed button is %d.\n", event.jbutton.button);	// 押されたボタンのIDを表示（0から）
            // ボタンIDに応じた処理
            switch(event.jbutton.button){
//弾を打つ
            case 3:
                fprintf(stderr, "4 push.\n");
                data.command = FOUR_COMMAND;
                data.cid = myid;
                data.state = GAME_CLEAR;
                send_data(&data, sizeof(CONTAINER), sock);
                stageFlag = 2;
                break;
            }
            break;
        }
    }
}

void send_data(void *data, int size, int sock) {
  if ((data == NULL) || (size <= 0)) {
        fprintf(stderr, "send_data(): data is illeagal.\n");
	exit(1);
  }

  if(write(sock, data, size) == -1) { //sockにdataのsizeバイト分書きこむ
      handle_error("write()"); //エラー
  }    
}







