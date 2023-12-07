// Fazer com que o projeto rode no linux e ajeitar o repo
#include <stdio.h>
#include <raylib.h>
#include <assert.h>
#include <string.h>

#define Largura 1280
#define Altura 720

enum Widgets
{
    BOTAO = 1,
    INPUT = 2
};

enum Estado
{
    ATIVO,
    PRESSIONADO
};


// Globais:
Font FontePadrao = (Font){};
Rectangle RetanguloUltimoWidget = (Rectangle){};
int TipoUltimoWidget = 0;
int EstadoUltimoWidget = 0;
int PosCaracteUltimoWidget = 0;

Rectangle fazer_linha(Rectangle *dimensoes_atual, int altura)
{
    Rectangle linha = (Rectangle){
        .x = dimensoes_atual->x,
        .y = dimensoes_atual->y,
        .width = dimensoes_atual->width,
        .height = altura
    };
    dimensoes_atual->y += altura; //esse mais um é para garantir que saímos do raio da linha
    dimensoes_atual->height -= altura; //esse mais um é para garantir que saímos do raio da linha
    return linha;
}

Rectangle fazer_coluna(Rectangle *dimensoes_atual, int grossura)
{
    Rectangle coluna = (Rectangle){
        .x = dimensoes_atual->x,
        .y = dimensoes_atual->y,
        .width = grossura,
        .height = dimensoes_atual->height
    };
    dimensoes_atual->x += grossura; //esse mais um é para garantir que saímos do raio da linha
    dimensoes_atual->width -= grossura;
    return coluna;
}

bool fazer_botao(const char *texto, Rectangle dimensoesMaximas)
{
    bool resultado = false;
    Vector2 posMouse = GetMousePosition();
    bool mouseEstaEmCimaDesseBotao = CheckCollisionPointRec(posMouse, dimensoesMaximas);
    int borda = mouseEstaEmCimaDesseBotao ? 3 : 2;
    int botaoEsquerdoDoMouseEstaPressionado = IsMouseButtonDown(0);
    
    if(botaoEsquerdoDoMouseEstaPressionado && mouseEstaEmCimaDesseBotao)
    {
        EstadoUltimoWidget = PRESSIONADO;
        borda++;
    }
    else if(
            mouseEstaEmCimaDesseBotao && 
            RetanguloUltimoWidget.x == dimensoesMaximas.x && 
            RetanguloUltimoWidget.y == dimensoesMaximas.y && 
            RetanguloUltimoWidget.width == dimensoesMaximas.width && 
            RetanguloUltimoWidget.height == dimensoesMaximas.height && 
            TipoUltimoWidget == BOTAO && 
            EstadoUltimoWidget == PRESSIONADO)
    {
       resultado = true; 
    }
    else if (mouseEstaEmCimaDesseBotao)
    {
        RetanguloUltimoWidget = dimensoesMaximas;
        TipoUltimoWidget = BOTAO;
        EstadoUltimoWidget = ATIVO;
    }


    DrawRectangleLinesEx(
            dimensoesMaximas,
            borda,
            RED
    );
    dimensoesMaximas.x += borda;
    dimensoesMaximas.y += borda;
    dimensoesMaximas.width -= borda * 2;
    dimensoesMaximas.height -= borda * 2;
    DrawRectangleRec(dimensoesMaximas, BLACK);
    int tamanhoTexto = dimensoesMaximas.height / 2 > 50 ? 50 : dimensoesMaximas.height / 2;
    DrawText(
            texto,
            dimensoesMaximas.x +  (dimensoesMaximas.width / 2) - MeasureText(texto, tamanhoTexto) / 2,
            dimensoesMaximas.y + (dimensoesMaximas.height / 2 - tamanhoTexto / 2), 
            tamanhoTexto,
            WHITE
    );
    return resultado;
}


void fazer_entrada_de_texto(char *texto, int capacidade, bool capacidade_fixa, Rectangle dimensoesMaximas)
{       
     int borda = 2;
     bool mouseEstaNesseInput = CheckCollisionPointRec(GetMousePosition(), dimensoesMaximas);
     bool mouseEstaPressionado = IsMouseButtonDown(0);
     Vector2 mousePos = GetMousePosition();
     bool clicaramNoInput = false;

     printf("%d\n", EstadoUltimoWidget);

     if(mouseEstaNesseInput) // esta no input
     {
        if(TipoUltimoWidget == INPUT) // o ultimo widget era um Input
        {

            if( // o ultimo widget era esse mesmo Input
                    RetanguloUltimoWidget.x == dimensoesMaximas.x &&  
                    RetanguloUltimoWidget.y == dimensoesMaximas.x &&
                    RetanguloUltimoWidget.width == dimensoesMaximas.width &&
                    RetanguloUltimoWidget.height == dimensoesMaximas.height
              )
              {
                    if (EstadoUltimoWidget == PRESSIONADO && mouseEstaPressionado == false) // o usuario tinha pressionado mais soltou
                    {
                        EstadoUltimoWidget = ATIVO;
                        clicaramNoInput = true;
                    }
              }
        }

        RetanguloUltimoWidget = dimensoesMaximas;
        TipoUltimoWidget = INPUT;
        EstadoUltimoWidget = ATIVO;
        if(mouseEstaPressionado) EstadoUltimoWidget = PRESSIONADO;
     }



     DrawRectangleLinesEx(
                dimensoesMaximas,
                borda,
                RED
     );
     
     dimensoesMaximas.x += borda;
     dimensoesMaximas.y += borda;
     dimensoesMaximas.width -= borda * 2;
     dimensoesMaximas.height -= borda * 2;


     int tamanhoTexto = dimensoesMaximas.height / 2;
     int espacamento = tamanhoTexto <= 10 ? 10 : tamanhoTexto / 10;
     dimensoesMaximas.y += tamanhoTexto + espacamento * 2 + 1;
     DrawText(texto, dimensoesMaximas.x, dimensoesMaximas.y, tamanhoTexto, BLACK);

     Vector2 posicaoHitBoxLetra = (Vector2){dimensoesMaximas.x, dimensoesMaximas.y};
    
     char str[2] = {'\0'};
     for(int i = 0; i < strlen(texto); i++)
     {
        str[0] = texto[i];
        Rectangle hitBox = (Rectangle){posicaoHitBoxLetra.x, posicaoHitBoxLetra.y , MeasureText(str, tamanhoTexto) + espacamento,MeasureText(str, tamanhoTexto) + espacamento * 3 + 1};
        posicaoHitBoxLetra.x += MeasureText(str, tamanhoTexto) + espacamento;

        if(clicaramNoInput && CheckCollisionPointRec(mousePos, hitBox))
        {
            PosCaracteUltimoWidget = i;
            puts("laitu");
        }

        if(PosCaracteUltimoWidget == i || PosCaracteUltimoWidget == -1)
        {
            Rectangle cursor = (Rectangle){hitBox.x + hitBox.width,hitBox.y,1,hitBox.height};
            DrawRectangleLinesEx(cursor, 1, BLUE);
        }

     }
}

int main(void)
{
    bool mostrarBotao = false;
    InitWindow(Largura, Altura,"GUI");
    FontePadrao = GetFontDefault();
    char buffer[20] = "Andrew";
    int capacidadeBuffer = 20;
    while(!WindowShouldClose())
    {
        Rectangle r = (Rectangle){0,0,Largura, Altura};
        BeginDrawing();
        ClearBackground(WHITE);
        fazer_entrada_de_texto(buffer, capacidadeBuffer,true,  fazer_linha(&r, 50));
        EndDrawing();
    }
}
