#include <bits/stdc++.h>
using namespace std;
const int MAXSIZE = 5;
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
void printHuffmanCodes(HuffmanNode *root, string code = "")
{
    if (root)
    {
        if (root->data != '\0')
        {
            cout << root->data << ": " << code << endl;
        }
        printHuffmanCodes(root->left, code + "0");
        printHuffmanCodes(root->right, code + "1");
    }
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
// Main function
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
struct GojoHash
{
    vector<int> areaInGoJoRes[MAXSIZE];
};
int main()
{
    string name = "aaabbcccDD";
    cout << getRes(name);
    return 0;
}