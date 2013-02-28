class StmtNode{
	
private:
	int astNode;
	int cfgNode;
	int nodeType;

public:
	StmtNode(int ast, int cfg, int type );
	int getAST();
    int getType();
	int getCFG();
	void setAST(int ast);
	void setType(int type);
	void setCFG(int cfg);


};