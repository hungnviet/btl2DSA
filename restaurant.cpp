#include "main.h"
int MAXSIZE;

/// build cay hulfman cho 1 thang

struct HuffmanNode
{
	char data;
	int frequency;
	int order;
	HuffmanNode *left;
	HuffmanNode *right;

	// Constructor
	HuffmanNode(char d, int freq, int count) : data(d), frequency(freq), left(nullptr), right(nullptr), order(count) {}
};
HuffmanNode *rotateRight(HuffmanNode *root)
{
	HuffmanNode *p = root->left;
	HuffmanNode *tmp = p->right;
	root->left = tmp;
	p->right = root;
	HuffmanNode *tmpp = root;
	tmpp->frequency = 0;
	if (tmpp->left)
	{
		tmpp->frequency += tmpp->left->frequency;
	}
	if (tmpp->right)
	{
		tmpp->frequency += tmpp->right->frequency;
	}
	HuffmanNode *temp = p;
	temp->frequency = 0;
	if (temp->left)
	{
		temp->frequency += temp->left->frequency;
	}
	if (temp->right)
	{
		temp->frequency += temp->right->frequency;
	}
	return p;
}
HuffmanNode *rotateLeft(HuffmanNode *root)
{
	HuffmanNode *p = root->right;
	HuffmanNode *tmp = p->left;
	root->right = tmp;
	p->left = root;
	HuffmanNode *tmpp = root;
	tmpp->frequency = 0;
	if (tmpp->left)
	{
		tmpp->frequency += tmpp->left->frequency;
	}
	if (tmpp->right)
	{
		tmpp->frequency += tmpp->right->frequency;
	}
	HuffmanNode *temp = p;
	temp->frequency = 0;
	if (temp->left)
	{
		temp->frequency += temp->left->frequency;
	}
	if (temp->right)
	{
		temp->frequency += temp->right->frequency;
	}
	return p;
}
int getHeight(HuffmanNode *root)
{
	if (!root)
	{
		return 0;
	}
	return 1 + max(getHeight(root->left), getHeight(root->right));
}
struct CompareNodes
{

	bool operator()(const HuffmanNode *lhs, const HuffmanNode *rhs) const
	{
		if (lhs->frequency != rhs->frequency)
		{
			return lhs->frequency > rhs->frequency;
		}
		else if (lhs->data != '\0' && rhs->data != '\0' && lhs->frequency == rhs->frequency)
		{
			if (islower(lhs->data) && isupper(rhs->data))
			{
				return false;
			}
			else if (isupper(lhs->data) && islower(rhs->data))
			{
				return true;
			}
			else
			{
				return lhs->data > rhs->data;
			}
		}
		else
		{
			return lhs->order > rhs->order;
		}
	}
};
HuffmanNode *checkAndRotate(HuffmanNode *root)
{
	if (!root)
	{
		return nullptr;
	}
	int balance = getHeight(root->left) - getHeight(root->right);
	if (balance > 1 && getHeight(root->left->left) - getHeight(root->left->right) >= 0)
	{
		return rotateRight(root);
	}
	if (balance > 1 && getHeight(root->left->left) < getHeight(root->left->right))
	{
		root->left = rotateLeft(root->left);
		return rotateRight(root);
	}
	if (balance < -1 && getHeight(root->right->right) >= getHeight(root->right->left))
	{
		return rotateLeft(root);
	}
	if (balance < -1 && getHeight(root->right->left) > getHeight(root->right->right))
	{
		root->right = rotateRight(root->right);
		return rotateLeft(root);
	}
	return root;
}
HuffmanNode *buildHuffmanTree(const vector<pair<char, int>> &frequencyTable)
{
	int count = 0;
	priority_queue<HuffmanNode *, vector<HuffmanNode *>, CompareNodes> pq;
	for (const auto &entry : frequencyTable)
	{
		HuffmanNode *node = new HuffmanNode(entry.first, entry.second, count);
		pq.push(node);
	}
	// Build the Huffman tree
	count++;
	while (pq.size() > 1)
	{

		HuffmanNode *leftChild = pq.top();
		pq.pop();
		HuffmanNode *rightChild = pq.top();
		pq.pop();
		HuffmanNode *internalNode = new HuffmanNode('\0', leftChild->frequency + rightChild->frequency, count);
		internalNode->left = leftChild;
		internalNode->right = rightChild;
		internalNode = checkAndRotate(internalNode);
		internalNode->left = checkAndRotate(internalNode->left);
		internalNode->right = checkAndRotate(internalNode->right);
		pq.push(internalNode);
		count++;
	}
	return pq.top();
}
char maHoa(char c, int n)
{
	char res;
	if (islower(c))
	{
		res = (c - 'a' + n) % 26 + 'a';
	}
	else if (isupper(c))
	{
		res = (c - 'A' + n) % 26 + 'A';
	}
	return res;
}
bool compare(pair<char, int> &a, pair<char, int> &b)
{
	if (a.second < b.second)
	{
		return true;
	}
	else if (a.second > b.second)
	{
		return false;
	}
	else if (a.second == b.second)
	{
		if ((islower(a.first) && islower(b.first)) || (isupper(a.first) && isupper(b.first)))
		{
			return a.first < b.first;
		}
		else
		{
			return islower(a.first);
		}
	}
}
vector<pair<char, int>> createFrequencyTable(string inputString)
{
	unordered_map<char, int> frequencyMap;
	for (char ch : inputString)
	{
		frequencyMap[ch]++;
	}
	vector<pair<char, int>> frequencyTable(frequencyMap.begin(), frequencyMap.end());
	sort(frequencyTable.begin(), frequencyTable.end(),
		 [&inputString](const pair<char, int> &a, const pair<char, int> &b)
		 {
			 return (a.second == b.second) ? inputString.find(a.first) < inputString.find(b.first) : a.second < b.second;
		 });
	return frequencyTable;
}
void encode(HuffmanNode *root, string str, unordered_map<char, string> &huffmanCode)
{
	if (root == nullptr)
		return;

	// found a leaf node
	if (!root->left && !root->right)
	{
		huffmanCode[root->data] = str;
	}

	encode(root->left, str + "0", huffmanCode);
	encode(root->right, str + "1", huffmanCode);
}
int binaryToDecimal(int n)
{
	int num = n;
	int dec_value = 0;

	int base = 1;

	int temp = num;
	while (temp)
	{
		int last_digit = temp % 10;
		temp = temp / 10;

		dec_value += last_digit * base;

		base = base * 2;
	}

	return dec_value;
}
HuffmanNode *rootOfLatestCustomer;
void delHulfmanTree(HuffmanNode *root)
{
	if (!root)
	{
		return;
	}
	delHulfmanTree(root->left);
	delHulfmanTree(root->right);
	delete root;
}
int getRes(string name)
{
	vector<pair<char, int>> freqencyTable = createFrequencyTable(name);

	unordered_map<char, int> freMap(freqencyTable.begin(), freqencyTable.end());
	// mã hóa rồi cộng dồn rồi sort lại
	vector<pair<char, int>> res;
	for (auto i : freqencyTable)
	{
		char c = maHoa(i.first, i.second);
		pair<int, char> tmp = {c, i.second};
		res.push_back(tmp);
	}

	map<char, int> tmp;
	for (auto i : res)
	{
		tmp[i.first] += i.second;
	}
	vector<pair<char, int>> Res(tmp.begin(), tmp.end()); /// vector để build cây hulfman
	sort(Res.begin(), Res.end(), compare);
	/// tên sau khi đc mã hóa
	string caesarName = "";
	for (char i : name)
	{
		char c = maHoa(i, freMap[i]);
		caesarName.push_back(c);
	}
	HuffmanNode *huffmanTreeRoot = buildHuffmanTree(Res);
	HuffmanNode *a = rootOfLatestCustomer;
	rootOfLatestCustomer = huffmanTreeRoot;
	delHulfmanTree(a);
	unordered_map<char, string> huffmanCode;
	encode(huffmanTreeRoot, "", huffmanCode);
	string str = "";
	for (char ch : caesarName)
	{
		str += huffmanCode[ch];
	}
	int result;
	string t = "";
	if (str.size() <= 10)
	{
		for (int i = str.size(); i > 0; i--)
		{
			t += str[i];
		}
	}
	else
	{
		for (int i = str.size() - 1; i > str.size() - 11; i--)
		{
			t += str[i];
		}
	}
	return binaryToDecimal(stoi(t));
}

/// mange name and result of each customer
struct Customer
{
	string name;
	int result;
	Customer(string a, int b)
	{
		name = a;
		result = b;
	}
	Customer()
	{
		name = "";
		result = 0;
	}
};
/// Build GOJO restaurent
struct nodeInAreaOfGoJo
{
	Customer cus;
	nodeInAreaOfGoJo *left;
	nodeInAreaOfGoJo *right;
	nodeInAreaOfGoJo() : left(nullptr), right(nullptr), cus(Customer()) {}
	nodeInAreaOfGoJo(string name, int res) : left(nullptr), right(nullptr), cus(name, res) {}
};
struct areaOfGoJo
{
	int idOfArea;
	int numOfCusInArea;
	nodeInAreaOfGoJo *root;
	vector<Customer> orderInputOfCustomer;
	areaOfGoJo()
	{
		numOfCusInArea = 0;
		root = nullptr;
		idOfArea = 0;
	}
	void addCusToAreaOfRoJo(string name, int res)
	{
		nodeInAreaOfGoJo *tmp = new nodeInAreaOfGoJo(name, res);

		if (!root)
		{
			root = tmp;
			numOfCusInArea++;
			return;
		}
		else
		{
			nodeInAreaOfGoJo *runner = root;
			while (true)
			{
				if (res < runner->cus.result)
				{
					if (runner->left == nullptr)
					{
						runner->left = tmp;
						numOfCusInArea++;
						return;
					}
					else
					{
						runner = runner->left;
					}
				}
				else if (res >= runner->cus.result)
				{
					if (runner->right == nullptr)
					{
						runner->right = tmp;
						numOfCusInArea++;
						return;
					}
					else
					{
						runner = runner->right;
					}
				}
			}
		}
		orderInputOfCustomer.push_back(tmp->cus);
	}
};
areaOfGoJo hashTableOfGoJo[100];
/// BUILD SUKANA RESTAURENT
struct nodeInHeapOfSukuna
{
	vector<Customer> customerInNodeOfSukuna;
	int ID;
	int numOfCusInNode;
	int order;
	nodeInHeapOfSukuna(int a, int b)
	{
		ID = a;
		numOfCusInNode = 0;
		order = b;
	}
};
int orderOfHeapNode = 0;
vector<nodeInHeapOfSukuna *> heap;
vector<int> orderUsedOfArea;
void reHeapDown(int i)
{
	int largest = i;
	int left = 2 * i + 1;
	int right = 2 * i + 2;
	if (left < heap.size() && heap[left]->numOfCusInNode < heap[largest]->numOfCusInNode)
	{
		largest = left;
	}
	if (right < heap.size() && heap[right]->numOfCusInNode < heap[largest]->numOfCusInNode)
	{
		largest = right;
	}
	if (largest != i)
	{
		swap(heap[i], heap[largest]);
		reHeapDown(largest);
	}
}
bool checkNodeExist(int ID)
{
	if (heap.size() == 0)
	{
		return false;
	}
	for (int i = 0; i < heap.size(); i++)
	{
		if (heap[i]->ID == ID)
		{
			return true;
		}
	}
	return false;
}
void addNodeToHeap(int ID)
{

	nodeInHeapOfSukuna *tmp = new nodeInHeapOfSukuna(ID, orderOfHeapNode + 1);
	heap.push_back(tmp);
	// reheap up
	int i = heap.size() - 1;
	while (i != 0 && heap[(i - 1) / 2]->numOfCusInNode > heap[i]->numOfCusInNode)
	{
		swap(heap[(i - 1) / 2], heap[i]);
		i = (i - 1) / 2;
	}
}
void addCustomer(string name, int result)
{
	int id = result % MAXSIZE + 1;

	Customer tmpCus(name, result);
	// update order used
	auto it = find(orderUsedOfArea.begin(), orderUsedOfArea.end(), id);
	if (it != orderUsedOfArea.end())
	{
		int index = distance(orderUsedOfArea.begin(), it);
		orderUsedOfArea.erase(orderUsedOfArea.begin() + index);
		orderUsedOfArea.insert(orderUsedOfArea.begin(), id);
	}
	else
	{
		orderUsedOfArea.insert(orderUsedOfArea.begin(), id);
	}

	/// add customer or node in restaurent
	if (checkNodeExist(id))
	{
		for (int i = 0; i < heap.size(); i++)
		{
			if (heap[i]->ID == id)
			{
				cout << id << endl;
				heap[i]->customerInNodeOfSukuna.push_back(tmpCus);
				heap[i]->numOfCusInNode++;
				reHeapDown(i);
				break;
			}
		}
	}
	else
	{
		addNodeToHeap(id);
		for (int i = 0; i < heap.size(); i++)
		{
			if (heap[i]->ID == id)
			{
				heap[i]->customerInNodeOfSukuna.push_back(tmpCus);
				heap[i]->numOfCusInNode++;
				reHeapDown(i);
				break;
			}
		}
	}
}

/// LAPSE FUNCTION
bool checkName(string name)
{
	set<char> s;
	for (int i = 0; i < name.length(); i++)
	{
		s.insert(name[i]);
	}
	if (s.size() < 3)
	{
		return false;
	}
	return true;
}
void LAPSE(string name)
{
	if (!checkName(name))
	{
		return;
	}

	int result = getRes(name);
	int id = result % MAXSIZE + 1;
	cout << "result: ";
	cout << result;
	cout << " ";
	cout << "ID: ";
	cout << id << endl;
	Customer tmpCus(name, result);
	if (result % 2 == 0)
	{
		addCustomer(name, result);
	}
	else
	{
		int ID = result % MAXSIZE + 1;
		hashTableOfGoJo[ID - 1].addCusToAreaOfRoJo(name, result);
	}
}

/// LIMITLESS FUNCTION TO PRINT THE GOJO RESTAURENT
void subLimitless(int num, areaOfGoJo arr[]);
void printLimitless(nodeInAreaOfGoJo *root);
void LIMITLESS(int num);
void LIMITLESS(int num)
{
	subLimitless(num, hashTableOfGoJo);
}
void subLimitless(int num, areaOfGoJo arr[])
{
	nodeInAreaOfGoJo *root = arr[num - 1].root;
	if (!root)
	{
		return;
	}
	printLimitless(root);
}
void printLimitless(nodeInAreaOfGoJo *root)
{
	if (!root)
	{
		return;
	}
	printLimitless(root->left);
	cout << root->cus.result << endl;
	printLimitless(root->right);
}
/// CLEAVE FUNCTION TO PRINT THE SUKUNA RESTAURENT
void subCleave(int start, int end);
void printEachAreaOfSukuna(nodeInHeapOfSukuna *node, int num);
void CLEAVE(int num)
{
	subCleave(0, num);
}
void subCleave(int index, int num)
{
	if (index < heap.size())
	{
		printEachAreaOfSukuna(heap[index], num);
		subCleave(2 * index + 1, num);
		subCleave(2 * index + 2, num);
	}
}
void printEachAreaOfSukuna(nodeInHeapOfSukuna *node, int num)
{
	if (num > node->numOfCusInNode)
	{
		num = node->numOfCusInNode;
	}
	int i = node->numOfCusInNode - 1;
	while (num > 0)
	{
		cout << node->ID;
		cout << "-";
		cout << node->customerInNodeOfSukuna[i].result << endl;
		num--;
		i--;
	}
}

/// KOKUSEN FUNCTION TO KICK THE GUESS IN GOJO restaurant
void subConvertToPost(vector<int> &res, nodeInAreaOfGoJo *root);
vector<int> arrayAfterConvertToPost(nodeInAreaOfGoJo *root)
{
	vector<int> res;
	subConvertToPost(res, root);
	return res;
}
void subConvertToPost(vector<int> &res, nodeInAreaOfGoJo *root)
{
	if (root)
	{
		subConvertToPost(res, root->left);
		subConvertToPost(res, root->right);
		res.push_back(root->cus.result);
	}
}
void factorial(int fact[], int n)
{
	fact[0] = 1;
	for (int i = 1; i <= n; i++)
	{
		fact[i] = fact[i - 1] * i;
	}
}
int nCr(int n, int r)
{
	int fact[n + 1];
	factorial(fact, n);
	return fact[n] / (fact[r] * fact[n - r]);
}
int counthoanvi(vector<int> &arr)
{
	int n = arr.size();
	if (n <= 2)
	{
		return 1;
	}
	vector<int> left;
	vector<int> right;
	int root = arr[0];
	for (int i = 0; i < n; i++)
	{
		if (arr[i] < root)
		{
			left.push_back(arr[i]);
		}
		else if (arr[i] > root)
		{
			right.push_back(arr[i]);
		}
	}
	int nleft = left.size();
	int nright = right.size();
	int countleft = counthoanvi(left);
	int countright = counthoanvi(right);
	return nCr(n - 1, nleft) * countleft * countright;
}
void removeInAreaOfGoJo(int ID, int Y);
void Kokusen()
{
	for (int i = 0; i < MAXSIZE; i++)
	{

		vector<int> arr = arrayAfterConvertToPost(hashTableOfGoJo[i].root);
		int rootforhvi = arr.back();
		arr.pop_back();
		arr.push_back(arr.front());
		arr.insert(arr.begin(), rootforhvi);
		int fact[arr.size()];
		int Y = counthoanvi(arr);
		removeInAreaOfGoJo(i + 1, Y);
	}
}
nodeInAreaOfGoJo *delNode(nodeInAreaOfGoJo *root, Customer tmp)
{
	if (!root)
	{
		return root;
	}
	if (root->cus.result > tmp.result)
	{
		root->left = delNode(root->left, tmp);
		return root;
	}
	else if (root->cus.result < tmp.result)
	{
		root->right = delNode(root->right, tmp);
		return root;
	}
	if (root->left == NULL)
	{
		nodeInAreaOfGoJo *temp = root->right;
		delete root;
		return temp;
	}
	else if (root->right == NULL)
	{
		nodeInAreaOfGoJo *temp = root->left;
		delete root;
		return temp;
	}
	else
	{

		nodeInAreaOfGoJo *succParent = root;

		// Find successor
		nodeInAreaOfGoJo *succ = root->right;
		while (succ->left != NULL)
		{
			succParent = succ;
			succ = succ->left;
		}

		if (succParent != root)
			succParent->left = succ->right;
		else
			succParent->right = succ->right;

		// Copy Successor Data to root
		root->cus = succ->cus;

		// Delete Successor and return root
		delete succ;
		return root;
	}
}
void removeInAreaOfGoJo(int ID, int Y)
{
	int n = hashTableOfGoJo[ID - 1].numOfCusInArea;
	if (n <= Y)
	{
		hashTableOfGoJo[ID - 1].root = nullptr;
		hashTableOfGoJo[ID - 1].numOfCusInArea = 0;
		hashTableOfGoJo[ID - 1].orderInputOfCustomer.clear();
	}
	else
	{
		for (int i = 0; i < Y; i++)
		{
			hashTableOfGoJo[ID - 1].root = delNode(hashTableOfGoJo[ID - 1].root, hashTableOfGoJo[ID - 1].orderInputOfCustomer[0]);
			hashTableOfGoJo[ID - 1].numOfCusInArea--;
			hashTableOfGoJo[ID - 1].orderInputOfCustomer.erase(hashTableOfGoJo[ID - 1].orderInputOfCustomer.begin() + 0);
		}
	}
}

/// KEITEIKEN FUNCTION TO REMOVE THE CUSTOMER IN SUKUNA RESTAURENT
void removeInAreaOfSukuna(int ID, int num);
struct areaAndNum
{
	int id;
	int num;
	areaAndNum(int a, int b)
	{
		id = a;
		num = b;
	}
};
bool compareNumOfCustomer(areaAndNum a, areaAndNum b)
{
	if (a.num < b.num)
	{
		return true;
	}
	else if (a.num > b.num)
	{
		return false;
	}
	else
	{
		auto ita = find(orderUsedOfArea.begin(), orderUsedOfArea.end(), a.id);
		auto itb = find(orderUsedOfArea.begin(), orderUsedOfArea.end(), b.id);
		int indexa = distance(orderUsedOfArea.begin(), ita);
		int indexb = distance(orderUsedOfArea.begin(), itb);
		return indexa >= indexb;
	}
}
void KEITEIKEN(int num)
{
	vector<areaAndNum> OrderOfNumCustomer;
	for (int i = 0; i < heap.size(); i++)
	{
		OrderOfNumCustomer.push_back(areaAndNum(heap[i]->ID, heap[i]->numOfCusInNode));
	}
	sort(OrderOfNumCustomer.begin(), OrderOfNumCustomer.end(), compareNumOfCustomer);

	if (num >= heap.size())
	{
		for (int i = 0; i < heap.size(); i++)
		{
			removeInAreaOfSukuna(OrderOfNumCustomer[i].id, OrderOfNumCustomer[i].num);
		}
		while (heap.size() > 0)
		{
			nodeInHeapOfSukuna *tmp = heap[heap.size() - 1];
			heap.pop_back();
			delete tmp;
		}
	}
	else
	{
		cout << "day ne" << endl;
		for (int i = 0; i < num; i++)
		{

			removeInAreaOfSukuna(OrderOfNumCustomer[i].id, num);
		}
	}
}
void removeInAreaOfSukuna(int ID, int num)
{
	for (int i = 0; i < heap.size(); i++)
	{
		if (heap[i]->ID == ID)
		{
			if (heap[i]->numOfCusInNode <= num)
			{
				/// ddooir no voi thang node cuoi truong heap
				nodeInHeapOfSukuna *tmp = heap[i];
				nodeInHeapOfSukuna *del = heap[heap.size() - 1];
				heap[i] == heap[heap.size() - 1];
				heap.pop_back();
				reHeapDown(i);
				// in ra
				for (int j = 0; j < tmp->numOfCusInNode; j++)
				{
					cout << tmp->customerInNodeOfSukuna[j].result;
					cout << "-";
					cout << tmp->ID << endl;
				}
				delete del;
				delete tmp;
			}
			else
			{
				for (int j = 0; j < num; j++)
				{
					cout << heap[i]->customerInNodeOfSukuna[0].name;
					cout << "-";
					cout << heap[i]->ID;
					heap[i]->numOfCusInNode--;
					heap[i]->customerInNodeOfSukuna.erase(heap[i]->customerInNodeOfSukuna.begin(), heap[i]->customerInNodeOfSukuna.begin() + 1);
				}
				/// reheap up
				int j = i;
				while (j != 0 && heap[(j - 1) / 2]->numOfCusInNode > heap[j]->numOfCusInNode)
				{
					swap(heap[(j - 1) / 2], heap[j]);
					j = (j - 1) / 2;
				}
			}
		}
	}
}

/// HAND function
void HAND();
void subHand(HuffmanNode *root);
void HAND()
{
	subHand(rootOfLatestCustomer);
}
void subHand(HuffmanNode *root)
{
	if (!root)
	{
		return;
	}
	subHand(root->left);
	if (root->data == '\0')
	{
		cout << root->frequency << endl;
	}
	else
		cout << root->data << endl;
	subHand(root->right);
}

void simulate(string filename)
{
	ifstream ss(filename);
	string str, maxsize, name, num;
	while (ss >> str)
	{
		if (str == "MAXSIZE")
		{
			ss >> maxsize;
			MAXSIZE = stoi(maxsize);
			cout << "MAXSIZE: ";
			cout << MAXSIZE << endl;
		}
		else if (str == "LAPSE")
		{
			ss >> name;
			cout << "LAPSE";
			cout << " ";
			cout << name << endl;
			LAPSE(name);
		}
		else if (str == "HAND")
		{
			cout << "HAND" << endl;
			HAND();
		}
		else if (str == "LIMITLESS")
		{
			ss >> num;
			int n = stoi(num);
			cout << "LIMITLESS ";
			cout << n << endl;
			LIMITLESS(n);
		}
		else if (str == "CLEAVE")
		{
			ss >> num;
			int n = stoi(num);
			cout << "CLEAVE ";
			cout << n << endl;
			CLEAVE(n);
		}
		else if (str == "KEITEIKEN")
		{
			ss >> num;
			int n = stoi(num);
			cout << "KEITEIKEN ";
			cout << n << endl;
			KEITEIKEN(n);
		}
		else if (str == "KOKUSEN")
		{
			cout << "KOKUSEN" << endl;
			Kokusen();
		}
	}
	return;
}