#define DEBUG 1

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

class ColorTiles{
public:

    float tileWidth, tileHeight;

    int lastTileSelectedCol, lastTileSelectedRow;

    glm::mat4 modelMatrix;

    VerticesObject* vertices;

    Tile matrixColors[numRows][numCols] = {};

	//   _______B_______
	//   |              |
	// A |              |  D
	//   |              |
	//   |______________|
	//          C

	//
	//left point
	float Ax;
	float Ay;

	//top point
	float Bx;
	float By;

	//bottom point
	float Cx;
	float Cy;

	//right point
	float Dx;
	float Dy;

    ColorTiles(float totalWidth, float totalHeight)
    {
        this->tileWidth = TILE_WIDTH;
        this->tileHeight = TILE_HEIGHT;

        this->modelMatrix = glm::mat4(1);

        this-> lastTileSelectedCol = -1;
        this-> lastTileSelectedRow = -1;

        this->setupVertices(tileWidth, tileHeight);

        this->createMatrixColors();
    }

    void setupVertices(float width, float height) {
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

	void setRelevantPoints(float row, float col) {
		/*
			Talvez ideal seria retornar uma struct com essas variáveis,
			em vez de elas serem atributos de classe
		*/
		float x0 = ((float)col)*tileWidth + ((float)row) *(tileWidth / 2.0f);
		float y0 = ((float)row)*tileHeight / 2.0f;

		this->Ax = x0;
		this->Ay = y0 + tileHeight / 2.0f;

		//top point
		this->Bx = x0 + tileWidth / 2.0f;
		this->By = y0;

		//bottom point
		this->Cx = x0 + tileWidth / 2.0f;
		this->Cy = y0 + tileHeight;

		//right point
		this->Dx = x0 + tileWidth;
		this->Dy = y0 + tileHeight / 2.0f;
	}

    void testCliqueMouse(double xPos,double yPos) {

        int rowClick = (int) (yPos / (tileHeight/2.0));
        int columnClick = (int) ((xPos - (rowClick * (tileWidth/2.0)))/tileWidth);

		/*Para garantir que apenas o mapa, ou areas bem proximas ao mapa sejam clicaveis*/
		if (rowClick < 0 || columnClick < 0)				return;
		if (rowClick > numRows || columnClick >= numCols)	return;

		printf("\nrowClick: %d columClick %d", rowClick, columnClick);

		this->setRelevantPoints(rowClick, columnClick);

        if(DEBUG==1){
            printf("\nxPos: %f", xPos);
            printf("\nyPos: %f", yPos);
            printf("\nRow: %d", rowClick);
            printf("\nColumn: %d\n", columnClick);
           // printf("\nx0: %f\n",x0);
           // printf("\ny0: %f\n",y0);
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
            if(DEBUG==1) printf("\nlado esquerda");

            if(testPointCollision(Ax,Ay, Bx, By,  Cx, Cy, xPos, yPos)) {
                if(DEBUG==1) printf("\nDEU BOM");
                if(rowClick==numRows){
                    return;
                }
            } else {
                if(DEBUG==1) printf("\nNAO DEU TAO BOM");
                if(rowClick==0){
                    return;
                }
                if(xPos<Bx  && yPos<Ay){
                    //caminha p cima
                    if(rowClick>0) {
                        rowClick--;
                    }
					/*
						Testa se o clique está dentro do novo tile
						Na esquerda poderia ter um if para ser feito apenas no col 0
						e na direita apenas no col lenght-1;
					*/
					this->setRelevantPoints(rowClick, columnClick);

					if (testPointCollision(Ax, Ay, Bx, By, Cx, Cy, xPos, yPos) == false) return;
                }
            }

        } else{
            //testar lado da direita
            if(DEBUG==1) printf("\nlado direita");
            if(testPointCollision(Dx,Dy, Bx, By,  Cx, Cy, xPos, yPos)) {
                if(DEBUG==1) printf("\nDEU BOM");
                if(rowClick==numRows){
                    return;
                }
            }else {
                if(DEBUG==1) printf("\nNAO DEU TAO BOM");
                if(rowClick==0){
                    return;
                }
                if(xPos>Bx  && yPos<Dy){
                    //caminha p cima e direita
                    if(columnClick<(numCols-1)&&rowClick>0) {
                        rowClick--;
                        columnClick++;
                    }
					/*
						Testa se o clique está dentro do novo tile
						Na esquerda poderia ter um if para ser feito apenas no col 0
						e na direita apenas no col lenght-1;
					*/
					this->setRelevantPoints(rowClick, columnClick);
					
					if (testPointCollision(Dx, Dy, Bx, By, Cx, Cy, xPos, yPos) == false) return;
                }
            }

        }

        if(this->lastTileSelectedCol>-1
                && this->lastTileSelectedRow>-1){
                this->matrixColors[lastTileSelectedRow][lastTileSelectedCol].isSelected = false;
        }

       
        if(matrixColors[rowClick][columnClick].isVisible){
            if (this->matrixColors[rowClick][columnClick].isSelected) {
                this->matrixColors[rowClick][columnClick].isSelected = false;
            } else {
                this->matrixColors[rowClick][columnClick].isSelected = true;
                this->lastTileSelectedRow = rowClick;
                this->lastTileSelectedCol =columnClick;
            }
        }

        
    }

    bool testPointCollision(float RefenceX,float RefenceY, float Bx,float By, float Cx,float Cy, float Px, float Py){
        double ABx = (double)Bx-(double)RefenceX;
        double ABy = (double)By-(double)RefenceY;
        double ABmodule = sqrt(pow(ABx,2)+pow(ABy,2));

        double normalABx = ABx / ABmodule;
        double normalABy = ABy / ABmodule;

        double ACx = (double)Cx-(double)RefenceX;
        double ACy = (double)Cy-(double)RefenceY;
        double ACmodule = sqrt(pow(ACx,2)+pow(ACy,2));

        double normalACx = ACx / ACmodule;
        double normalACy = ACy / ACmodule;

        double APx = (double)Px-(double)RefenceX;
        double APy = (double)Py-(double)RefenceY;
        double APmodule = sqrt(pow(APx,2)+pow(APy,2));

        double normalAPx = APx / APmodule;
        double normalAPy = APy / APmodule;

        double theta = acos(normalABx * normalAPx + normalABy * normalAPy);
        double alpha = acos(normalABx * normalACx + normalABy * normalACy);
        double betha = acos(normalACx * normalAPx + normalACy * normalAPy);

        //bool collide = alpha == (theta + betha);
        bool collide = 0.001>abs(alpha - (theta + betha));
        return collide;
    }


    /*
       Função que responde às teclas pressionadas
   */
    void keyboard_reaction(int keys[1024]) {

        if (keys[GLFW_KEY_UP] == 1) {
            do_a_movement('n');
        } else if (keys[GLFW_KEY_DOWN] == 1) {
            do_a_movement('s');
        } else if (keys[GLFW_KEY_LEFT] == 1) {
            do_a_movement('w');
        } else if (keys[GLFW_KEY_RIGHT] == 1) {
            do_a_movement('e');
        }
    }

    void do_a_movement(char a){
        if(a=='n'){//up
            if(this->lastTileSelectedRow>0){
                this->matrixColors[this->lastTileSelectedRow][this->lastTileSelectedCol].isSelected = false;

                this->lastTileSelectedRow = this->lastTileSelectedRow-1;
                this->lastTileSelectedCol = this->lastTileSelectedCol;

                this->matrixColors[this->lastTileSelectedRow][this->lastTileSelectedCol].isSelected = true;
            }
        } else if(a=='s'){//down
            if(this->lastTileSelectedRow<(numRows-1)){
                this->matrixColors[this->lastTileSelectedRow][this->lastTileSelectedCol].isSelected = false;

                this->lastTileSelectedRow = this->lastTileSelectedRow+1;
                this->lastTileSelectedCol = this->lastTileSelectedCol;

                this->matrixColors[this->lastTileSelectedRow][this->lastTileSelectedCol].isSelected = true;
            }
        } else if(a=='w') {//left
            if(this->lastTileSelectedCol>0&&this->lastTileSelectedRow<(numRows-1)){
                this->matrixColors[this->lastTileSelectedRow][this->lastTileSelectedCol].isSelected = false;

                this->lastTileSelectedRow = this->lastTileSelectedRow+1;
                this->lastTileSelectedCol = this->lastTileSelectedCol-1;

                this->matrixColors[this->lastTileSelectedRow][this->lastTileSelectedCol].isSelected = true;
            }
        } else if(a=='e') {//right
            if(this->lastTileSelectedCol<(numCols-1)&&this->lastTileSelectedRow>0){
                this->matrixColors[this->lastTileSelectedRow][this->lastTileSelectedCol].isSelected = false;

                this->lastTileSelectedRow = this->lastTileSelectedRow-1;
                this->lastTileSelectedCol = this->lastTileSelectedCol+1;

                this->matrixColors[this->lastTileSelectedRow][this->lastTileSelectedCol].isSelected = true;
            }
        }


    }


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

