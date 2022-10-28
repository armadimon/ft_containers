/* 2020-1 컴퓨터알고리즘과 실습 02분반
2015112121 컴퓨터공학과 최성준
알고리즘 실습 7주차 문제3: Red_Black_Tree (Insert만 구현)
*/
#include <iostream>
#include <string>
using namespace std;

// 레드블랙트리의 색깔을 지정하기 위한 상수값 표현
namespace Color 
{
	const enum NODE_COLOR : bool { BLACK = false, RED = true };
}

// 레드블랙트리의 노드를 의미하는 클래스
class Node 
{
private:
	int key;	// 노드의 unique한 key값
	bool color;	// 노드의 색깔
	Node* parent, * left, * right;	// 부모노드, 왼쪽 자식노드, 오른쪽 자식노드를 가리킨다.
	friend class RedblackTree;		// 레드블랙트리 클래스를 friend로 지정.
public:
	explicit Node(const int& paramKey = NULL, const bool paramColor = Color::BLACK,
		Node* paramParent = nullptr, Node* paramLeft = nullptr, Node* paramRight = nullptr)
		: key(paramKey), color(paramColor), parent(paramParent), left(paramLeft), right(paramRight) {}
};

// 레드블랙트리를 의미하는 클래스
class RedblackTree 
{
private:	// 멤버 변수
	int numOfElement;	// 트리 내 존재하는 원소 개수
	Node* nil;	// BST, 234 트리의 dummyHead 역할을 대신하는 nil.

private:	// 멤버 함수 (private)
	Node* getGrandNode(Node* cur);	// 조부모 노드를 반환해주는 함수
	Node* getUncleNode(Node* cur);	// 부모의 Sibling(삼촌)을 반환해주는 함수
	void leftRotate(Node* cur);		// 트리를 left rotate하는 함수
	void rightRotate(Node* cur);	// 트리를 right rotate하는 함수
	void insertFix(Node* cur);		// 레드블랙트리의 특성을 만족하도록 트리 구조 수정하는 함수
	void insertNode(Node* root, const int& data);	// 노드 삽입 함수
	void preOrderTraverse(Node* root, std::string* stringBuilder, std::string padding, 
		const std::string& pointer, const bool hasRightChild) const;
	std::string showTree(Node* root) const;	// 트리 pre-order traverse 출력 함수

public:		// 멤버 함수 (public)
	explicit RedblackTree() : numOfElement(0), nil(new Node(-1, Color::BLACK)) {
		nil->left = nil->right = nil->parent = nil;
		// 초기화 리스트에서 nil(new Node(-1, Color::BLACK, nil, nil, nil) 하면 안됨.
		// nil을 생성할 때는 nil이 nullptr이므로 멤버 포인터 변수가 nullptr이 되기 때문이다.
	}
	void insertNode(const int& data) { insertNode(nil, data); }
	std::string showTree() const { return showTree(nil->right); }
};

// 조부모 노드를 반환해주는 함수
Node* RedblackTree::getGrandNode(Node* cur)
{
	// 사실 이 조건문은 없어도 된다. 모든 노드는 생성시 nullptr대신 nil로 연결되기 때문.
	if (cur == nullptr || cur->parent == nullptr) return nullptr;
	return cur->parent->parent;
}

// 부모의 Sibling(삼촌)을 반환해주는 함수
Node* RedblackTree::getUncleNode(Node* cur)
{
	Node* tempGrand(getGrandNode(cur));
	if (tempGrand == nullptr) return nullptr;
	// 조부모의 오른쪽 자식노드가 부모노드면 왼쪽이 삼촌노드다.
	if (cur->parent == tempGrand->left) return tempGrand->right;
	// 조부모의 왼쪽 자식노드가 부모노드면 오른쪽이 삼촌노드다.
	else return tempGrand->left;
}

// 트리를 left rotate하는 함수
void RedblackTree::leftRotate(Node* cur)
{
	// 과정 1) 좌회전시 오른쪽 자식노드를 가리키는 임시 포인터 생성.
	Node* temp = cur->right;
	// 과정 2) cur과 temp를 분리시키면서, temp의 왼쪽 자식노드를 cur이 인수인계함.
	cur->right = temp->left;
	// 과정 3) 방금 인수인계한게 nil이 아니라면, 인수인계한 노드의 부모를 cur로 지정.
	if (temp->left != nil) temp->left->parent = cur;
	// 과정 4) temp의 부모를 cur에서 cur->parent로 바꿔준다.
	temp->parent = cur->parent;
	// 과정 5) cur의 현재 상태에 따라서 temp를 이어주는 경우의 수가 나뉨.
	if (cur->parent == nil) nil->right = temp;
	else if (cur == cur->parent->left) cur->parent->left = temp;
	else cur->parent->right = temp;
	// 과정 6) 이제 모든 사전준비가 끝났으니 temp의 왼쪽 자식노드를 cur로 지정함.
	temp->left = cur;
	// 과정 7) 이제 cur의 부모가 temp가 되면 좌회전 완료.
	cur->parent = temp;
}

// 트리를 right rotate하는 함수
void RedblackTree::rightRotate(Node* cur)
{
	// left Rotate에서 right과 left만 바뀌고 나머지는 똑같다.
	Node* temp = cur->left;
	cur->left = temp->right;

	if (temp->right != nil) temp->right->parent = cur;

	temp->parent = cur->parent;

	if (cur->parent == nil) nil->right = temp;
	else if (cur == cur->parent->left) cur->parent->left = temp;
	else cur->parent->right = temp;

	temp->right = cur;
	cur->parent = temp;
}

// 레드블랙트리의 특성을 만족하도록 트리 구조 수정하는 함수
void RedblackTree::insertFix(Node* cur)
{	// 부모가 root 노드일 때 까지 반복한다. (root노드는 흑색)
	while (cur->parent->color == Color::RED) 
	{
		Node* grandNode(getGrandNode(cur));		// 조부모 노드
		Node* uncleNode(getUncleNode(cur));		// 삼촌 노드
		if (cur->parent == grandNode->left) 
		{	// [CASE 1]: 부모도 삼촌도 모두 빨간색인 경우
			if (uncleNode->color == Color::RED) 
			{	// 부모와 삼촌 그리고 조부모 노드의 색깔을 반전시켜준다.
				cur->parent->color = uncleNode->color = Color::BLACK;
				grandNode->color = Color::RED;
				cur = grandNode;	// cur을 조부모로 올려서 다음 loop를 준비한다.
			}
			else 
			{// [CASE 2]: 삼촌이 흑색이고, cur이 오른쪽 자식노드인 경우
				if (cur == cur->parent->right)
				{ 
					cur = cur->parent;	// cur을 한 칸 올리고,
					leftRotate(cur);	// 좌회전 한다.
				}
				// [CASE 3]: 삼촌이 흑색이고, cur이 왼쪽 자식노드인 경우
				cur->parent->color = !cur->parent->color;	// 부모 색깔 반전
				grandNode->color = !grandNode->color;		// 조부모 색깔 반전
				rightRotate(grandNode);	// 우회전 한다. (주의! grandNode로 우회전)
			}
		}
		else 
		{
			if (uncleNode->color == Color::RED) 
			{
				cur->parent->color = uncleNode->color = Color::BLACK;
				grandNode->color = Color::RED;
				cur = grandNode;
			}
			else
			{
				if (cur == cur->parent->left)
				{
					cur = cur->parent;
					leftRotate(cur);
				}
				cur->parent->color = !cur->parent->color;
				grandNode->color = !grandNode->color;
				rightRotate(grandNode);
			}
		}
	}
 	nil->right->color = Color::BLACK;
}

void RedblackTree::insertNode(Node* root, const int& data)
{
	Node* parent, * tail;
	parent = nil;				// 일반적으로 parent가 nil을 가리킨다.
	tail = nil->right;			// 일반적으로 tail이 root 노드를 가리킨다.
	while (tail != nil)			// [과정 1]: 삽입할 위치를 찾는다.
	{		
		parent = tail;			// parent를 tail로 한 칸 내려야 tail을 이동시킬 수 있다.
		int var = tail->key;
		if (data == var)		// [오류]: 트리에 이미 값이 존재할 경우 uniqueness에 위반된다.
		{		
			cout << "[ERROR] Duplicated input value!" << endl;
			return;
		}
		// 삽입할 값이 현재 노드의 key보다 작다면 왼쪽 자식노드로 내려간다.
		else if (data < var) tail = tail->left;
		// 삽입할 값이 현재 노드의 key보다 크다면 오른쪽 자식노드로 내려간다.
		else tail = tail->right;
	}	// 반복문을 탈출함 = 삽입할 위치가 parent에 저장되었음을 의미한다.
	tail = new Node(data, Color::RED, parent, nil, nil);	// new 함수를 이용해 새로운 노드를 만든다.
	if (parent == nil)	// root 노드를 삽입하는 경우,
	{
		parent->right = tail;	// nil->right = tail로 root노드로 만들어주고
		tail->color = Color::BLACK;	// root 노드는 반드시 반드시 흑색이어야 한다.
	}
	// 삽입할 값이 삽입 위치의 노드의 key보다 작다면 왼쪽 자식노드에 삽입한다.
	// 삽입할 값이 삽입 위치의 노드의 key보다 크다면 오른쪽 자식노드에 삽입한다.
	else if (data < parent->key) parent->left = tail;
	else parent->right = tail;

	this->numOfElement++;		// 트리의 노드 개수를 1 증가시킨다.
	insertFix(tail);		// 레드블랙트리의 특성을 만족하도록 매 삽입연산 때 트리 구조를 수정한다.
}

void RedblackTree::preOrderTraverse(Node* root, std::string* stringBuilder, std::string padding, const std::string& pointer, const bool hasRightChild) const
{
	std::string* paddingBuilder;	// 이 loop에서 stringBuilder에 append할 문자열을 의미한다.
	if (root == nil) return;
	else 
	{
		paddingBuilder = new std::string(padding);
		stringBuilder->append("\n").append(padding).append(pointer);
		if (root->color == Color::BLACK) stringBuilder->append("B");
		else stringBuilder->append("R");
		stringBuilder->append(to_string(root->key));

		if (hasRightChild) paddingBuilder->append("│  ");
		else paddingBuilder->append("   ");
	}

	// [재귀함수]
	preOrderTraverse(root->left, stringBuilder, *paddingBuilder, "├──", root->right != nullptr);
	preOrderTraverse(root->right, stringBuilder, *paddingBuilder, "└──", false);

	delete paddingBuilder;		// 메모리 누수를 막자.
}

std::string RedblackTree::showTree(Node* root) const
{
	if (this->numOfElement == 0) 
	{	// [오류]: 비어있는 BST에 대한 'show'함수 호출은 오류상황이다.
		cout << "[ERROR] Can't perform 'show' function on empty tree!" << endl;
		return "";
	}

	std::string* str = new std::string();	// BST 전체를 preorder로 출력할 문자열 포인터를 생성한다.
	if (root->color == Color::BLACK) str->append("B");
	else str->append("R");
	str->append(to_string(root->key));		// 우선 문자열에 root 노드의 key를 넣어준다.

	// [본격적인 재귀 함수 시작 부분] 
	preOrderTraverse(root->left, str, "", "├──", root->right != nullptr);
	preOrderTraverse(root->right, str, "", "└──", false);

	// 재귀함수가 끝나면 문자열에는 전체 BST의 출력문이 들어있게 된다.
	std::string returnStr = *str;	// 문자열 포인터를 삭제해주기 위해 지역변수에 복사한다.
	delete str;						// 문자열 포인터를 delete해서 메모리 누수를 막는다.
	return returnStr;				// 지역변수를 넘겨주고 프로그램 종료시 컴파일러가 삭제하게 맡긴다.
}

int main(void) 
{
	RedblackTree* myTree = new RedblackTree();
	if (myTree == nullptr) 
	{	// [오류]: 메모리 할당 문제
		cout << "[ERROR] Tree memory allocation problem" << endl;
		return -1;
	}

	cout << "-------------------------------------" << endl;
	cout << "Commands:" << endl;
	cout << "   '+Key': Insert element" << endl;
	cout << "      'Q': Quit the test program" << endl;
	cout << "--------------------------------------" << endl;
	cout << "※ In tree draw, '├──key' is left child, '└──key' is right child ※" << endl;

	std::string userStr;

	while (true) 
	{
		cout << myTree->showTree() << endl;		// 매 loop마다 트리를 출력하도록 설정한다.

		cout << "Command: ";
		cin >> userStr;

		char userOption = userStr[0];		// 사용자 입력값의 첫 글자는 연산 option을 의미한다.
		userStr.erase(0, 1);				// atoi 함수를 사용하기 위해 첫 글자를 삭제한다.
		int userVar = std::atoi(userStr.c_str());	// 사용자 입력 문자열을 정수로 바꾼다.
		// [오류]: 유효하지 않은 연산자일경우 오류이므로 다시 입력하게끔 유도한다.
		if (userOption != '+' && userOption != 'Q' && userOption != 'q') 
		{
			cout << "[ERROR] Wrong command! Input command again please" << endl;
			continue;
		}
		// 사용자가 입력한 옵션에 따라 대응하는 함수를 호출한다.
		switch (userOption) 
		{
		case '+':
			myTree->insertNode(userVar);
			break;
		case 'Q': case 'q':
			cout << "Quit program!" << endl;
			delete myTree;
			return 0;
		}
	}
}
