#include <iostream>
#include <rapidjson/document.h>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include <map>
using namespace std;
using namespace rapidjson;

struct WordEntry
{
    string tos;
    string definition;
    string engTrans;
    string references;
    string crossrefs;
    vector<string> tables;
};

struct DictEntry
{
    string word;
    string pronunciaton;
    vector<WordEntry> entries;
};

string numToString(int i)
{
    switch(i)
    {
        case 1:
            return "one";
        case 2:
            return "two";
        case 3:
            return "three";
        case 4:
            return "four";
        case 5:
            return "five";
        case 6:
            return "six";
        default:
            return "";
    }
}

template <typename TKey, typename TVal>
vector<pair<TKey, TVal>> sortMapByKey(map<TKey, TVal>& dict)
{
    vector<pair<TKey, TVal>> vec;

    for (typename map<TKey, TVal>::iterator it = dict.begin(); it != dict.end(); it++)
    {
        vec.push_back(pair<TKey, TVal>(it->first, it->second));
    }

    sort(vec.begin(), vec.end(), [](const pair<TKey, TVal>& a, const pair<TKey, TVal>& b)->bool {
        return a.first < b.first;
    });

    return vec;
}

string parseLangData(Document& doc)
{
    string beginning = "\\documentclass[12pt]{article}\n\\usepackage{fullpage}\n\\usepackage{color}\n\n\\newcommand{\\entry}[7]{\\markboth{#1}{#1}\\textbf{#1} \\ \\textipa{#2}\\ \\textsuperscript{1}\\textsc{#3}. \\ {#4}\\ \\textit{#5}\\ see also: \\textit{#6}. \\ {#7}}\n\\newcommand{\\entrytwo}[5]{; \\textsuperscript{2}\\textsc{#1} \\ {#2} \\ \\textit{#3} \\ see also: \\textit{#4}. \\ {#5}}\n\\newcommand{\\entrythree}[5]{; \\textsuperscript{3}\\textsc{#1} \\ {#2} \\ \\textit{#3} \\ see also: \\textit{#4}. \\ {#5}}\n\\newcommand{\\entryfour}[5]{; \\textsuperscript{4}\\textsc{#1} \\ {#2} \\ \\textit{#3} \\ see also: \\textit{#4}. \\ {#5}}\n\\newcommand{\\entryfive}[5]{; \\textsuperscript{5}\\textsc{#1} \\ {#2} \\ \\textit{#3} \\ see also: \\textit{#4}. \\ {#5}}\n\\newcommand{\\entrysix}[5]{; \\textsuperscript{6}\\textsc{#1} \\ {#2} \\ \\textit{#3} \\ see also: \\textit{#4}. \\ {#5}}\n\\newcommand{\\phrase}[3]{; \\textbf{#1} \\ {#2} \\textit{#3}}\n\\newcommand{\\note}[1]{; \\textbf{note}: {#1}}\n\\newcommand{\\sentence}[2]{\\textsuperscript{#1}{#2}}\n\n\\usepackage{vowel}\n\n\\usepackage{hyperref}\n\\hypersetup{\n\tcolorlinks=true,\n\tlinkcolor=blue,\n\tfilecolor=magenta,\n\turlcolor=cyan,\n}\n\n\\usepackage{tipa}\n\n\\usepackage{graphicx}\n\n\\usepackage{amssymb}\n\\let\\oldemptyset\\emptyset\n\\let\\emptyset\\varnothing\n\n\\usepackage{multicol}\n\n\\usepackage{expex}\n\n\\setlength{\\parindent}{0cm}\n\n\\usepackage{microtype}\n\n";

    string documentTitle = doc["documentTitle"].GetString();
    string documentSubtitle = "", documentAuthor = "";

    if (doc.HasMember("documentSubtitle"))
    {
        documentSubtitle = doc["documentSubtitle"].GetString();
    }

    if (doc.HasMember("author"))
    {
        documentAuthor = doc["author"].GetString();
    }

    string preamble = "\\title{\n\t" + documentTitle + "\\\\\n\t\\large " + documentSubtitle + "}\n\\author{" + documentAuthor + "}\n\n";

    GenericArray<false, rapidjson::Value> langs = doc["langs"].GetArray();
    string docBegin = "\\begin{document}\n\t\\pagenumbering{gobble}\n\t\\maketitle\n\t\\newpage\n\n\t\\pagenumbering{roman}\n\t\\tableofcontents\n\t\\newpage\n\n\t\\pagenumbering{arabic}\n";
    vector<string> langStrings;

    for (Value* iter = langs.Begin(); iter != langs.End(); ++iter)
    {
        string langString = "";
        GenericObject<false, rapidjson::Value> lang = iter->GetObject();
        string name = lang["name"].GetString();
        string engName = lang["engName"].GetString();
        GenericArray<false, rapidjson::Value> abbrevs = lang["abbrevs"].GetArray();

        langString += ("\t\\section{" + name + " (" + engName + ")}\n\t\t\\subsection{" + name + "-English Dictionary}\n\t\t\t\\subsubsection{Abbreviations}\n\t\t\t\t\\begin{table}[ht]\n\t\t\t\t\t\\centering\n\t\t\t\t\t\\begin{tabular}{r|l}\n");

        // Parse abbrevs
        for (Value* aIter = abbrevs.Begin(); aIter != abbrevs.End(); ++aIter)
        {
            GenericObject<false, rapidjson::Value> abbrev = aIter->GetObject();
            string aAbbrev = abbrev["abbrev"].GetString();
            string aLong = abbrev["long"].GetString();
            langString += ("\t\t\t\t\t\t\\textsc{" + aAbbrev + "} & " + aLong + " \\\\\n");
        }

        langString += ("\t\t\t\t\t\\end{tabular}\n\t\t\t\t\\end{table}\n\t\t\t\\subsubsection{Format of Entries}\n\t\t\t\t\\entry{entry}{[ipa]}{grammar}{definition}{references}{crossrefs}{tables} \\entrytwo{grammar}{definition}{references}{crossrefs}{tables} \\entrythree{grammar}{definition}{references}{crossrefs}{tables} \\entryfour{grammar}{definition}{references}{crossrefs}{tables} \\entryfive{grammar}{definition}{references}{crossrefs}{tables} \\entrysix{grammar}{definition}{references}{crossrefs}{tables} \\phrase{phrase}{definition}{example} \\note{note}\n\t\t\t\\newpage\n\t\t\t\\begin{multicols}{2}\n");

        GenericArray<false, rapidjson::Value> dictionary = lang["dictionary"].GetArray();

        // Parse dictionary
        vector<DictEntry> entries;
        
        for (Value* wIter = dictionary.Begin(); wIter != dictionary.end(); ++wIter)
        {
            DictEntry entry;
            GenericObject<false, rapidjson::Value> dEntry = wIter->GetObject();
            entry.word = dEntry["word"].GetString();
            entry.pronunciaton = dEntry["pronunciation"].GetString();
            GenericArray<false, rapidjson::Value> dEntries = dEntry["entries"].GetArray();

            for (Value* dIter = dEntries.Begin(); dIter != dEntries.end(); ++dIter)
            {
                WordEntry wordEntry;
                GenericObject<false, rapidjson::Value> wEntry = dIter->GetObject();
                wordEntry.tos = wEntry["tos"].GetString();
                wordEntry.definition = wEntry["definition"].GetString();
                wordEntry.engTrans = wEntry["engTrans"].GetString();

                if (wEntry.HasMember("references"))
                {
                    wordEntry.references = wEntry["references"].GetString();
                }
                else
                {
                    wordEntry.references = "";
                }
                
                if (wEntry.HasMember("crossrefs"))
                {
                    wordEntry.crossrefs = wEntry["crossrefs"].GetString();
                }
                else
                {
                    wordEntry.crossrefs = "";
                }
                
                if (wEntry.HasMember("tables"))
                {
                    GenericArray<false, rapidjson::Value> wtables = wEntry["tables"].GetArray();

                    for (Value* wtable = wtables.begin(); wtable != wtables.end(); ++wtable)
                    {
                        wordEntry.tables.push_back(wtable->GetString());
                    }
                }
                
                entry.entries.push_back(wordEntry);
            }

            entries.push_back(entry);
        }

        sort(entries.begin(), entries.end(), [](const DictEntry& a, const DictEntry& b)->bool {
            return a.word < b.word;
        });

        char currentStartingChar = '\0';
        map<string, pair<string, string>> engToLang;

        for (vector<DictEntry>::iterator deIter = entries.begin(); deIter != entries.end(); deIter++)
        {
            if (deIter->word[0] != currentStartingChar)
            {
                langString += ("\t\t\t\t\\subsubsection{" + string(1, toupper(deIter->word[0])) + "}\n");
                currentStartingChar = deIter->word[0];
            }

            langString += ("\t\t\t\t\t\\entry{" + deIter->word + "}{[" + deIter->pronunciaton + "]}{" + deIter->entries[0].tos + "}{" + deIter->entries[0].definition + "}{" + deIter->entries[0].references + "}{" + deIter->entries[0].crossrefs + "}{");

            for (int i = 0; i < (int)deIter->entries[0].tables.size(); i++)
            {
                langString += ("\\ref{tab:" + deIter->entries[0].tables[i] + "}");

                if (i < ((int)deIter->entries[0].tables.size() - 1))
                {
                    langString += ",";
                }
            }

            langString += "}} ";
            engToLang.insert(pair<string, pair<string, string>>(deIter->entries[0].engTrans, pair<string, string>(deIter->word, deIter->entries[0].tos)));

            for (int i = 1; i < (int)deIter->entries.size(); i++)
            {
                string num = numToString(i);
                langString += ("\\entry" + num + " {" + deIter->entries[i].tos + "}{" + deIter->entries[i].definition + "}{" + deIter->entries[i].references + "}{" + deIter->entries[i].crossrefs + "}{");

                for (int j = 0; j < (int)deIter->entries[i].tables.size(); j++)
                {
                    langString += ("\\ref{tab:" + deIter->entries[i].tables[j] + "}");

                    if (j < ((int)deIter->entries[i].tables.size() - 1))
                    {
                        langString += ",";
                    }
                }

                langString += "}} ";
                engToLang.insert(pair<string, pair<string, string>>(deIter->entries[i].engTrans, pair<string, string>(deIter->word, deIter->entries[i].tos)));
            }

            langString += "\n\n";
        }

        langString += ("\t\t\t\\end{multicols}\n\t\t\\newpage\n\t\t\\subsection{English-" + name + " Dictionary}\n\t\t\t\\begin{multicols}{2}\n");
        currentStartingChar = '\0';
        vector<pair<string, pair<string, string>>> engToLangSorted = sortMapByKey<string, pair<string, string>>(engToLang);

        for (vector<pair<string, pair<string, string>>>::iterator etlIter = engToLangSorted.begin(); etlIter != engToLangSorted.end(); etlIter++)
        {
            if (etlIter->first[0] != currentStartingChar)
            {
                langString += ("\t\t\t\t\\subsubsection{" + string(1, toupper(etlIter->first[0])) + "}\n");
                currentStartingChar = etlIter->first[0];
            }

            langString += ("\t\t\t\t\t\\entry{" + etlIter->first + "}{}{" + etlIter->second.second + "}{" + etlIter->second.first + "}{}{}{}\n\n");
        }

        langString += ("\t\t\t\\end{multicols}\n\t\t\\newpage\n\t\t\\subsection{Grammar Tables}\n");

        GenericArray<false, rapidjson::Value> tables = lang["tables"].GetArray();

        // Parse tables
        for (Value* table = tables.Begin(); table != tables.End(); ++table)
        {
            langString += ("\t\t\t\t\\begin{table}[ht]\n\t\t\t\t\t\\centering\n\t\t\t\t\t\\begin{tabular}{");
            GenericObject<false, rapidjson::Value> tableObj = table->GetObject();
            string tableName = tableObj["name"].GetString();
            string id = tableObj["id"].GetString();
            vector<string> rowLabels;
            GenericArray<false, rapidjson::Value> tColLabels = tableObj["colLabels"].GetArray(), tRowLabels = tableObj["rowLabels"].GetArray(), rows = tableObj["rows"].GetArray();
            string colStyles = tableObj["colStyles"].GetString();
            langString += (colStyles + "}\n\t\t\t\t\t\t");

            for (Value* colLabel = tColLabels.Begin(); colLabel != tColLabels.End(); ++colLabel)
            {
                string label = colLabel->GetString();
                langString += (" & " + label);
            }

            langString += (" \\\\ \\hline\n");

            for (Value* rowLabel = tRowLabels.begin(); rowLabel != tRowLabels.End(); ++rowLabel)
            {
                rowLabels.push_back(rowLabel->GetString());
            }

            int i = 0;

            for (Value* r = rows.Begin(); r != rows.End(); ++r)
            {
                langString += ("\t\t\t\t\t\t" + rowLabels[i++]);
                GenericArray<false, rapidjson::Value> row = r->GetArray();

                for (Value* rr = row.Begin(); rr != row.End(); ++rr)
                {
                    string data = rr->GetString();
                    langString += (" & " + data);
                }

                langString += " \\\\\n";
            }

            langString += ("\t\t\t\t\t\\end{tabular}\n\t\t\t\t\t\\caption{" + tableName + "}\n\t\t\t\t\t\\label{tab:" + id + "}\n\t\t\t\t\\end{table}\n");
        }

        langStrings.push_back(langString);
    }

    string langString = "";

    for (string ls : langStrings)
    {
        langString += ls;
    }

    return beginning + preamble + docBegin + langString + "\\end{document}";
}

int main(int argc, char* argv[])
{
    argc--; argv++;

    if (argc == 0)
    {
        cout << "Please specify the json file you'd like to read." << endl;
        return 0;
    }
    else if (argc == 1)
    {
        string filename = string(argv[0]);

        if (find(filename.begin(), filename.end(), '.') != filename.end())
        {
            if (filename.substr(filename.find_last_of('.')) == ".json")
            {
                cout << "Converting file..." << endl;
                string data, line;
                ifstream file(filename);

                if (file.is_open())
                {
                    while (getline(file, line))
                    {
                        data += line;
                    }

                    file.close();
                    Document doc;
                    doc.Parse(data.c_str());
                    string langString = parseLangData(doc);
                    ofstream newFile((filename.substr(0, filename.find_last_of('.')) + ".tex").c_str());

                    if (newFile.is_open())
                    {
                        newFile << langString;
                        newFile.close();
                    }
                    else
                    {
                        cout << "Could not open output file " << (filename.substr(0, filename.find_last_of('.')) + ".tex") << endl;
                        return 1;
                    }

                    cout << "Done." << endl;
                    return 0;
                }
                else
                {
                    cout << "Could not open file " << filename << endl;
                    return 1;
                }

                return 0;
            }
            else
            {
                cout << "Please specify a json file that you'd like to read." << endl;
                return 0;
            }
        }
        else
        {
            cout << "Please specify a json file that you'd like to read." << endl;
            return 0;
        }
    }
    else
    {
        cout << "Please only specify the json file you'd like to read." << endl;
        return 0;
    }
}