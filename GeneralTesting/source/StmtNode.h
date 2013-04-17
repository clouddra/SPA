class StmtNode{
	
private:
	int astNode;
	int cfgNode;
	int nodeType;

public:
    /**
	 * Constructor with all values initialised
	 */
	StmtNode(int ast, int cfg, int type);

    /**
	 * Getter function for the astNode
	 */
	int getAST();

    /**
	 * Getter function for the nodeType
	 */
    int getType();

    /**
	 * Getter function for the cfgNode
	 */
	int getCFG();

    /**
	 * Setter function for the astNode
	 */
	void setAST(int ast);

    /**
	 * Setter function for the nodeType
	 */
	void setType(int type);

    /**
	 * Setter function for the cfgNode
	 */
	void setCFG(int cfg);
};