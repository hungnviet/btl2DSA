#include <bits/stdc++.h>
using namespace std;
const int MAXSIZE = 5;
/// build cay hulfman cho 1 thang
struct HuffmanNode
{
    char data;
    int frequency;
    HuffmanNode *left;
    HuffmanNode *right;

    // Constructor
    HuffmanNode(char d, int freq) : data(d), frequency(freq), left(nullptr), right(nullptr) {}
};
struct CompareNodes
{
    bool operator()(HuffmanNode *lhs, HuffmanNode *rhs)
    {
        return lhs->frequency >= rhs->frequency;
    }
};
HuffmanNode *buildHuffmanTree(const vector<pair<char, int>> &frequencyTable)
{
    priority_queue<HuffmanNode *, vector<HuffmanNode *>, CompareNodes> pq;
    for (const auto &entry : frequencyTable)
    {
        HuffmanNode *node = new HuffmanNode(entry.first, entry.second);
        pq.push(node);
    }

    // Build the Huffman tree
    while (pq.size() > 1)
    {
        HuffmanNode *leftChild = pq.top();
        pq.pop();
        HuffmanNode *rightChild = pq.top();
        pq.pop();
        HuffmanNode *internalNode = new HuffmanNode('\0', leftChild->frequency + rightChild->frequency);
        internalNode->left = leftChild;
        internalNode->right = rightChild;
        pq.push(internalNode);
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
    return (a.second == b.second) ? a.first < b.first : a.second < b.second;
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
void encode(HuffmanNode *root, string str,
            unordered_map<char, string> &huffmanCode)
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
    nodeInAreaOfGoJo() : left(nullptr), right(nullptr)
    {
        cus = Customer();
    }
    nodeInAreaOfGoJo(string name, int res)
    {
        cus = Customer(name, res);
        left = nullptr;
        right = nullptr;
    }
};
struct areaOfGoJo
{
    int idOfArea;
    int numOfCusInArea;
    nodeInAreaOfGoJo *root;
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
    }
};
areaOfGoJo hashTableOfGoJo[MAXSIZE];

/// BUILD SUKANA RESTAURENT
struct nodeInHeapOfSukuna
{
    vector<Customer> customerInNodeOfSukuna;
    int ID;
    int numOfCusInNode;

    nodeInHeapOfSukuna(int a)
    {
        ID = a;
        numOfCusInNode = 0;
    }
};
vector<nodeInHeapOfSukuna *> heap;
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
    nodeInHeapOfSukuna *tmp = new nodeInHeapOfSukuna(ID);
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
    if (checkNodeExist(id))
    {
        for (int i = 0; i < heap.size(); i++)
        {
            if (heap[i]->ID == id)
            {
                heap[i]->customerInNodeOfSukuna.push_back(tmpCus);
                heap[i]->numOfCusInNode++;
                reHeapDown(i);
                return;
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
                return;
            }
        }
    }
}

/// LAPSE FUNCTION
void LAPSE(string name)
{
    int result = getRes(name);
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
int main()
{

    return 0;
}