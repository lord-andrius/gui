// Fazer com que o projeto rode no linux e ajeitar o repo
#include <stdio.h>
#include <raylib.h>
#include <assert.h>

#define Largura 1280
#define Altura 720

enum Widgets
{
    BOTAO = 1
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

int tamanho_string(const char *texto)
{
    int contador = 0;
    char caractere;
    int resultado = 0;
    while((caractere = texto[contador++]) != '\0')
    {
        GlyphInfo info = GetGlyphInfo(FontePadrao, caractere);
        resultado += info.image.width;
    }
    return resultado;
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
            dimensoesMaximas.x +  (dimensoesMaximas.width / 2) - tamanho_string(texto),
            dimensoesMaximas.y + (dimensoesMaximas.height / 2 - tamanhoTexto / 2), 
            tamanhoTexto,
            WHITE
    );
    return resultado;
}

int main(void)
{
    bool mostrarBotao = false;
    InitWindow(Largura, Altura,"GUI");
    FontePadrao = GetFontDefault();
    while(!WindowShouldClose())
    {
        Rectangle r = (Rectangle){0,0,Largura, Altura};
        BeginDrawing();
        ClearBackground(WHITE);
        if(fazer_botao("Clique Aqui Para ver Um botao extra", fazer_linha(&r, 50)))
            mostrarBotao = true;
        fazer_botao("Clique Aqui", fazer_linha(&r, 50));
        Rectangle coluna = fazer_coluna(&r, 50);
        fazer_botao("Clique Aqui", fazer_linha(&coluna, 50));
        fazer_botao("Clique Aqui", fazer_linha(&coluna, 50));
        fazer_botao("Clique Aqui", coluna);
        if(mostrarBotao)
        {
            bool resultado = fazer_botao("Clique Aqui Para esconder o botao extra",fazer_linha(&r, r.height));
            mostrarBotao = !resultado;
        }
        EndDrawing();
    }
}
