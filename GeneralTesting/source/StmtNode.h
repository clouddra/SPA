class StmtNode{
	
private:
	int astNode;
	int cfgNode;
	int nodeType;
	int cfgBipNode;
public:
	StmtNode(int ast, int cfg, int cfgBipNode, int type);
	int getAST();
    int getType();
	int getCFG();
	int getCFGBip();
	void setAST(int ast);
	void setType(int type);
	void setCFG(int cfg);
	void setCFGBip(int cfgBip);

};