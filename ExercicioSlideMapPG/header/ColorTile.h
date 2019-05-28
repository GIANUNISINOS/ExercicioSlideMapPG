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

#define numRows 15
#define numCols 10
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

    void testCliqueMouse(double xPos,double yPos) {

        int rowClick = (int) (yPos / (tileHeight/2.0));
        printf("Row: %d",rowClick);
        int columnClick = (int) ((xPos - (rowClick * (tileWidth/2.0)))/tileWidth);
        printf("\nColumn: %d\n",columnClick);


        float x0 = (float)columnClick*tileWidth  + (float)rowClick *(tileWidth/2.0f) ;
        float y0 = (float)rowClick*tileHeight/2.0f ;

        //left point
        float Ax = x0;
        float Ay = y0 + tileHeight/2.0f;

        //top point
        float Bx = x0 + tileWidth/2.0f;
        float By = y0;

        //bottom point
        float Cx = x0 + tileWidth/2.0f;
        float Cy = y0 + tileHeight;

        //right point
        float Dx = x0 + tileWidth;
        float Dy = y0 + tileHeight/2.0f;


        if(1==1){
            printf("\nx0: %f\n",x0);
            printf("\ny0: %f\n",y0);

            printf("\nleftPoint x %f",Ax);
            printf("\nleftPoint y %f\n",Ay);

            printf("\ntopPointX x %f",Bx);
            printf("\ntopPointY y %f\n",By);


            printf("\nbottomPointX x %f",Cx);
            printf("\nbottomPointY y %f\n",Cy);


            printf("\nrightPointX x %f",Dx);
            printf("\nrightPointY y %f\n",Dy);
        }

        if(xPos < Bx){
            //testar lado da esquerda
            printf("lado esquerda");

            float ABx = Bx-Ax;
            float ABy = By-Ay;
            float ABmodule = sqrt(pow(ABx,2)+pow(ABy,2));

            printf("\nABx x %f",Dx);
            printf("\nrightPointY y %f\n",Dy);

            float ACx = Cx-Ax;
            float ACy = Cy-Ay;
            float ACmodule = sqrt(pow(ACx,2)+pow(ACy,2));

            float APx = xPos-Ax;
            float APy = yPos-Ay;
            float APmodule = sqrt(pow(APx,2)+pow(APy,2));
            printf("lado esquerda");


        } else{
            //testar lado da direita
            printf("lado direita");
        }











        if(matrixColors[rowClick][columnClick].isVisible){
            if (matrixColors[rowClick][columnClick].isSelected) {
                matrixColors[rowClick][columnClick].isSelected = false;
            } else {
                matrixColors[rowClick][columnClick].isSelected = true;
            }
        }

    }

    void collision(){

    }

    /**

    float TriangleArea(rowClick,columnClick){

        this->tileWidth
        this->tileHeight




    }
*/
    void draw(Shader *shaderProgram) {

        // Define shaderProgram como o shader a ser utilizado
        shaderProgram->UseProgramShaders();

        // Define em quais vertices sera desenhado pelo shader
        vertices->bind(shaderProgram);

        for (int row = 0; row < numRows; row++) {
            for (int col = 0; col < numCols; col++) {

                //float xi = col*tileWidth;
                //float yi = row*tileHeight;
                float xi = col*tileWidth  + row *(tileWidth/2.0f) ;
                float yi = row*tileHeight/2.0f ;

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
