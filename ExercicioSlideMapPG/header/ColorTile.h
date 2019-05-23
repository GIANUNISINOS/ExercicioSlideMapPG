//
// Created by Gian Boschetti on 2019-05-13.
//

#ifndef GAMEOFCOLORS_COLORTILE_H
#define GAMEOFCOLORS_COLORTILE_H

class Tile {
public:
    glm::vec3 colorsRGB;
    bool isVisible;
    bool isSelected;

    Tile() {
        isVisible = true;
        isSelected = false;
    }

    void setColor(int R,int G,int B){
        float r;
        float g;
        float b;

        r = R/255.0f;
        g = G/255.0f;
        b = B/255.0f;

        colorsRGB = glm::vec3(r,g,b);
    }

    void generateColor(int row){
        bool isPair = (row%2==0);

        float r;
        float g;
        float b;

        if(isPair){
            r = 12/255.0f;
            g = 195/255.0f;
            b = 233/255.0f;
        } else{
            r = 23/255.0f;
            g = 79/255.0f;
            b = 214/255.0f;
        }
        colorsRGB = glm::vec3(r,g,b);
    }
};

#define numRows 40
#define numCols 20
class ColorTiles{
public:

    float tileWidth, tileHeight;

    glm::mat4 modelMatrix;

    VerticesObject* vertices;

    Tile matrixColors[numRows][numCols] = {};

    ColorTiles(float totalWidth, float totalHeight)
    {
        this->tileWidth = totalWidth / (float)numCols;
        this->tileHeight = totalHeight / (float)numRows;

        this->modelMatrix = glm::mat4(1);

        this->setupVertices(tileWidth, tileHeight);

        this->createMatrixColors();
    }

    void setupVertices(float width, float height) {
        /*
            Comeca centralizado no zero
        */
        float verticesCoordinates[] = {
                // positions
                width / 2.0f,   0.0f,           0.0f,	// TOP
                width ,         height / 2.0f,  0.0f,	// RIGHT
                width  / 2.0f,  height,         0.0f,	// BOTTOM
                0.0f,           height / 2.0f,  0.0f	// LEFT

        };

        vertices = new VerticesObject(verticesCoordinates, 12);
    }

    void createMatrixColors(){
        for (int row = 0; row < numRows; row++) {
              for (int col = 0; col < numCols; col++) {
                    Tile t = Tile();
                    t.generateColor(row);
                    matrixColors[row][col] = t;
                  }
            }
    }

    int testCliqueMouse(float xPos,float yPos) {
        int counter = 0;
        int columnClick = (int) (xPos / tileWidth);
        int rowClick = (int) (yPos / tileHeight);
        if(matrixColors[rowClick][columnClick].isVisible){
            if (matrixColors[rowClick][columnClick].isSelected) {
                matrixColors[rowClick][columnClick].isSelected = false;
            } else {
                matrixColors[rowClick][columnClick].isSelected = true;
            }
        }

        return counter;
    }

    void draw(Shader *shaderProgram) {

        // Define shaderProgram como o shader a ser utilizado
        shaderProgram->UseProgramShaders();

        // Define em quais vertices sera desenhado pelo shader
        vertices->bind(shaderProgram);

        for (int row = 0; row < numRows; row++) {
            for (int col = 0; col < numCols; col++) {

                float xi = col*tileWidth;
                float yi = row*tileHeight;

                modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(xi, yi, 0.0));

                //define aonde desenhar
                glUniformMatrix4fv(
                        glGetUniformLocation(shaderProgram->Program, "matrix_OBJ"), 1,
                        GL_FALSE, glm::value_ptr(modelMatrix));



                //seleciona o tile a ser desenhado
                Tile tile = matrixColors[row][col];


                if(!tile.isSelected){
                    glUniform3fv(
                            glGetUniformLocation(shaderProgram->Program, "colorValues"), 1,
                            glm::value_ptr(tile.colorsRGB));
                } else {
                    glUniform3fv(
                            glGetUniformLocation(shaderProgram->Program, "colorValues"), 1,
                            glm::value_ptr(glm::vec3(10,255,10)));
                }


                // Define em quais vertices sera desenhado pelo shader
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            }
        }

    }

};

#endif //GAMEOFCOLORS_COLORTILE_H
