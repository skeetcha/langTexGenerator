#include <string>
#include <vector>
#include <map>
#include <rapidjson/document.h>
#include <algorithm>
#include <iostream>
#include <fstream>
using namespace std;
using namespace rapidjson;

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

namespace Latex
{
    enum DocumentType
    {
        Article
    };

    enum FontSizeType
    {
        Point,
        None
    };

    struct FontSize
    {
        int amount;
        FontSizeType unit;

        FontSize()
        {
            amount = -1;
            unit = FontSizeType::None;
        }
    };

    struct LatexCommand
    {
        string name;
        int argNum;
        vector<string> output;

        LatexCommand(string n, int an, vector<string> o)
        {
            name = n;
            argNum = an;
            output = o;
        }
    };

    enum PageNumberingType
    {
        Gobble,
        Arabic,
        Roman
    };

    string markBoth(string first, string second)
    {
        string r = "\\markboth{";
        r += first;
        r += "}{";
        r += second;
        r += "}";
        return r;
    }

    string comArg(int num)
    {
        string r = "#";
        char buffer[3];
        _itoa_s(num, buffer, (size_t)3, 10);
        r += buffer;
        return r;
    }

    string boldText(string text)
    {
        string r = "\\textbf{";
        r += text;
        r += "}";
        return r;
    }

    string ipaText(string text)
    {
        string r = "\\textipa{";
        r += text;
        r += "}";
        return r;
    }

    string superScript(string text)
    {
        string r = "\\textsuperscript{";
        r += text;
        r += "}";
        return r;
    }

    string scText(string text)
    {
        string r = "\\textsc{";
        r += text;
        r += "}";
        return r;
    }

    string italicText(string text)
    {
        string r = "\\textit{";
        r += text;
        r += "}";
        return r;
    }

    string setPageNumbering(PageNumberingType type)
    {
        string r = "\\pagenumbering{";

        switch (type)
        {
        case PageNumberingType::Gobble:
            r += "gobble";
            break;
        case PageNumberingType::Arabic:
            r += "arabic";
            break;
        case PageNumberingType::Roman:
            r += "roman";
            break;
        default:
            r += "arabic";
            break;
        }

        r += "}";
        return r;
    }

    string makeTitle()
    {
        return "\\maketitle";
    }

    string newPage()
    {
        return "\\newpage";
    }

    string createTOS()
    {
        return "\\tableofcontents";
    }

    string newSection(string name)
    {
        return "\\section{" + name + "}";
    }

    string newSubSection(string name)
    {
        return "\\subsection{" + name + "}";
    }

    string newSubSubSection(string name)
    {
        return "\\subsubsection{" + name + "}";
    }

    string makeTableWithLabel(vector<vector<string>> data, vector<bool> hasHLines, string tableSettings, string tabularSettings, bool isCentered, string label, string id)
    {
        string r = "\\begin{table}[";
        r += tableSettings;
        r += "]\n";

        if (isCentered)
        {
            r += "\\centering\n";
        }

        r += "\\begin{tabular}{";
        r += tabularSettings;
        r += "}\n";

        for (int i = 0; i < (int)data.size(); i++)
        {
            for (int j = 0; j < (int)data[i].size(); j++)
            {
                r += data[i][j];

                if (j < ((int)data[i].size() - 1))
                {
                    r += "&";
                }
            }

            r += "\\\\";

            if (hasHLines[i])
            {
                r += " \\hline";
            }

            r += "\n";
        }

        r += "\\end{tabular}\n";
        
        if (label != "")
        {
            r += "\\caption{";
            r += label;
            r += "}\n";
        }

        if (id != "")
        {
            r += "\\label{tab:";
            r += id;
            r += "}\n";
        }

        r += "\\end{table}";
        return r;
    }

    string makeTable(vector<vector<string>> data, vector<bool> hasHLines, string tableSettings, string tabularSettings, bool isCentered)
    {
        return makeTableWithLabel(data, hasHLines, tableSettings, tabularSettings, isCentered, "", "");
    }

    string newEntry(string entry, string ipa, string grammar, string def, string refs, string xrefs, vector<string> tables)
    {
        string r = "\\entry{" + entry + "}{[" + ipa + "]}{" + grammar + "}{" + def + "}{" + refs + "}{" + xrefs + "}{";

        for (int i = 0; i < (int)tables.size(); i++)
        {
            r += "\\ref{tab:";
            r += tables[i];
            r += "}";

            if (i < ((int)tables.size() - 1))
            {
                r += ", ";
            }
        }

        r += "}";
        return r;
    }

    string newAdditionalEntry(int entryNum, string grammar, string def, string refs, string xrefs, vector<string> tables)
    {
        string r = "\\entry" + numToString(entryNum) + "{" + grammar + "}{" + def + "}{" + refs + "}{" + xrefs + "}{";

        for (int i = 0; i < (int)tables.size(); i++)
        {
            r += "\\ref{tab:";
            r += tables[i];
            r += "}";

            if (i < ((int)tables.size() - 1))
            {
                r += ", ";
            }
        }

        r += "}";
        return r;
    }

    string newPhrase(string phrase, string def, string ex)
    {
        return "\\phrase{" + phrase + "}{" + def + "}{" + ex + "}";
    }

    string newNote(string note)
    {
        return "\\note{" + note + "}";
    }

    string beginMulticols(int numCols)
    {
        char buffer[3];
        _itoa_s(numCols, buffer, (size_t)3, 10);
        return "\\begin{multicols}{" + string(buffer) + "}";
    }

    string endMulticols()
    {
        return "\\end{multicols}";
    }

    class Document
    {
    private:
        DocumentType type;
        FontSize fontSize;
        vector<string> packages;
        vector<LatexCommand> commands;
        vector<string> packageSettings;
        vector<string> docSettings;
        string title;
        string subtitle;
        string author;
        vector<string> docCommands;
    public:
        Document(DocumentType t)
        {
            type = t;
            title = "";
            subtitle = "";
        }

        Document(DocumentType t, int size, FontSizeType fst)
        {
            type = t;
            fontSize.amount = size;
            fontSize.unit = fst;
        }
        
        void usePackage(string packageName)
        {
            packages.push_back(packageName);
        }

        void addCommand(string commandName, int argNum, vector<string> output)
        {
            commands.push_back(LatexCommand(commandName, argNum, output));
        }

        void setPkgSettings(string commandName, map<string, string> options)
        {
            string com = "\\";
            com += commandName;
            com += "{";

            for (map<string, string>::iterator iter = options.begin(); iter != options.end(); iter++)
            {
                com += iter->first;
                com += "=";
                com += iter->second;
                com += ",";
            }

            com += "}";
            packageSettings.push_back(com);
        }

        void setDocumentSetting(string command)
        {
            docSettings.push_back(command);
        }

        void setTitle(string t)
        {
            title = t;
        }

        void setSubtitle(string s)
        {
            subtitle = s;
        }

        void setAuthor(string a)
        {
            author = a;
        }

        void setDocument(vector<string> dc)
        {
            docCommands = dc;
        }

        string toString()
        {
            string data = "";

            // Setup document class
            {
                data += "\\documentclass";
                
                if (fontSize.unit == FontSizeType::None)
                {
                    data += "[11pt]";
                }
                else
                {
                    data += "[";
                    char buffer[3];
                    _itoa_s(fontSize.amount, buffer, (size_t)3, 10);
                    data += string(buffer);
                    
                    switch (fontSize.unit)
                    {
                    case FontSizeType::Point:
                        data += "pt";
                        break;
                    default:
                        data += "pt";
                        break;
                    }

                    data += "]";
                }

                switch (type)
                {
                case DocumentType::Article:
                    data += "{article}";
                    break;
                default:
                    data += "{article}";
                    break;
                }

                data += "\n";
            }

            // Add packages
            {
                for (string package : packages)
                {
                    data += "\\usepackage{";
                    data += package;
                    data += "}\n";
                }
            }

            // Add commands
            {
                for (LatexCommand command : commands)
                {
                    data += "\\newcommand{\\";
                    data += command.name;
                    data += "}[";
                    char buffer[3];
                    _itoa_s(command.argNum, buffer, (size_t)3, 10);
                    data += buffer;
                    data += "]{";
                    
                    for (string output : command.output)
                    {
                        data += output;
                    }

                    data += "}\n";
                }
            }

            // Add package settings
            {
                for (string pkgCom : packageSettings)
                {
                    data += pkgCom;
                    data += "\n";
                }
            }

            // Add document settings
            {
                for (string docSetting : docSettings)
                {
                    data += docSetting;
                    data += "\n";
                }
            }

            // Add preamble
            {
                data += "\\title{";
                data += title;
                data += "\\\\\n\t\\large ";
                data += subtitle;
                data += "}\n";
                data += "\\author{";
                data += author;
                data += "}\n";
            }

            // Begin document
            {
                data += "\\begin{document}\n";
            }

            // Add body of document
            {
                for (string command : docCommands)
                {
                    data += command;
                    data += "\n";
                }
            }

            // End document
            {
                data += "\\end{document}";
            }

            return data;
        }
    };
}

string parseLangData(Document& jDoc)
{
    Latex::Document doc(Latex::DocumentType::Article, 12, Latex::FontSizeType::Point);
    doc.usePackage("fullpage");
    doc.usePackage("color");

    // Add commands
    {
        doc.addCommand(
            "entry",
            7,
            {
                Latex::markBoth(Latex::comArg(1), Latex::comArg(1)),
                Latex::boldText(Latex::comArg(1)), 
                " \\ ",
                Latex::ipaText(Latex::comArg(2)),
                "\\ ",
                Latex::superScript("1"),
                Latex::scText(Latex::comArg(3)),
                ". \\ {",
                Latex::comArg(4),
                "}\\ ",
                Latex::italicText(Latex::comArg(5)),
                "\\ see also: ",
                Latex::italicText(Latex::comArg(6)),
                ". \\ {",
                Latex::comArg(7),
                "}"
            }
        );
        doc.addCommand(
            "entrytwo",
            5,
            {
                "; ",
                Latex::superScript("2"),
                Latex::scText(Latex::comArg(1)),
                " \\ {",
                Latex::comArg(2),
                "} \\ ",
                Latex::italicText(Latex::comArg(3)),
                " \\ see also: ",
                Latex::italicText(Latex::comArg(4)),
                ". \\ {",
                Latex::comArg(5),
                "}"
            }
        );
        doc.addCommand(
            "entrythree",
            5,
            {
                "; ",
                Latex::superScript("3"),
                Latex::scText(Latex::comArg(1)),
                " \\ {",
                Latex::comArg(2),
                "} \\ ",
                Latex::italicText(Latex::comArg(3)),
                " \\ see also: ",
                Latex::italicText(Latex::comArg(4)),
                " \\ {",
                Latex::comArg(5),
                "}"
            }
        );
        doc.addCommand(
            "entryfour",
            5,
            {
                "; ",
                Latex::superScript("4"),
                Latex::scText(Latex::comArg(1)),
                " \\ {",
                Latex::comArg(2),
                "} \\ ",
                Latex::italicText(Latex::comArg(3)),
                " \\ see also: ",
                Latex::italicText(Latex::comArg(4)),
                " \\ {",
                Latex::comArg(5),
                "}"
            }
        );
        doc.addCommand(
            "entryfive",
            5,
            {
                "; ",
                Latex::superScript("5"),
                Latex::scText(Latex::comArg(1)),
                " \\ {",
                Latex::comArg(2),
                "} \\ ",
                Latex::italicText(Latex::comArg(3)),
                " \\ see also: ",
                Latex::italicText(Latex::comArg(4)),
                " \\ {",
                Latex::comArg(5),
                "}"
            }
        );
        doc.addCommand(
            "entrysix",
            5,
            {
                "; ",
                Latex::superScript("6"),
                Latex::scText(Latex::comArg(1)),
                " \\ {",
                Latex::comArg(2),
                "} \\ ",
                Latex::italicText(Latex::comArg(3)),
                " \\ see also: ",
                Latex::italicText(Latex::comArg(4)),
                " \\ {",
                Latex::comArg(5),
                "}"
            }
        );
        doc.addCommand(
            "phrase",
            3,
            {
                "; ",
                Latex::boldText(Latex::comArg(1)),
                " \\ {",
                Latex::comArg(2),
                "} ",
                Latex::italicText(Latex::comArg(3))
            }
        );
        doc.addCommand(
            "note",
            1,
            {
                "; ",
                Latex::boldText("note"),
                ": {",
                Latex::comArg(1),
                "}"
            }
        );
    }
    doc.usePackage("hyperref");
    doc.setPkgSettings(
        "hypersetup",
        {
            {"colorlinks", "true"},
            {"linkcolor", "blue"},
            {"filecolor", "magenta"},
            {"urlcolor", "cyan"}
        }
    );
    doc.usePackage("tipa");
    doc.usePackage("graphicx");
    doc.usePackage("multicol");
    doc.usePackage("expex");
    doc.setDocumentSetting("\\setlength{\\parindent}{0cm}");
    doc.usePackage("microtype");
    doc.setTitle(jDoc["documentTitle"].GetString());

    if (jDoc.HasMember("documentSubtitle"))
    {
        doc.setSubtitle(jDoc["documentSubtitle"].GetString());
    }

    if (jDoc.HasMember("author"))
    {
        doc.setAuthor(jDoc["author"].GetString());
    }

    GenericArray<false, rapidjson::Value> langs = jDoc["langs"].GetArray();

    vector<string> commands;
    commands.push_back(Latex::setPageNumbering(Latex::PageNumberingType::Gobble));
    commands.push_back(Latex::makeTitle());
    commands.push_back(Latex::newPage());
    commands.push_back(Latex::setPageNumbering(Latex::PageNumberingType::Roman));
    commands.push_back(Latex::createTOS());
    commands.push_back(Latex::newPage());
    commands.push_back(Latex::setPageNumbering(Latex::PageNumberingType::Arabic));

    for (Value* iter = langs.Begin(); iter != langs.End(); ++iter)
    {
        vector<string> langComms;
        GenericObject<false, rapidjson::Value> lang = iter->GetObject();
        string name = lang["name"].GetString();
        string engName = lang["engName"].GetString();
        GenericArray<false, rapidjson::Value> abbrevs = lang["abbrevs"].GetArray();

        langComms.push_back(Latex::newSection(name + " (" + engName + ")"));
        langComms.push_back(Latex::newSubSection(name + "-English Dictionary"));
        langComms.push_back(Latex::newSubSubSection("Abbreviations"));
        
        vector<vector<string>> aTableData;
        vector<bool> aHasHLines;

        for (Value* aIter = abbrevs.Begin(); aIter != abbrevs.End(); ++aIter)
        {
            vector<string> aRow;
            GenericObject<false, rapidjson::Value> abbrev = aIter->GetObject();
            aRow.push_back(abbrev["abbrev"].GetString());
            aRow.push_back(abbrev["long"].GetString());
            aTableData.push_back(aRow);
            aHasHLines.push_back(false);
        }

        langComms.push_back(Latex::makeTable(aTableData, aHasHLines, "ht", "r|l", true));
        langComms.push_back(Latex::newSubSubSection("Format of Entries"));
        langComms.push_back(Latex::newEntry("entry", "ipa", "grammar", "definition", "references", "crossrefs", {"tables"}) + " " + Latex::newAdditionalEntry(2, "grammar", "definition", "references", "crossrefs", {"tables"}) + " " + Latex::newAdditionalEntry(3, "grammar", "definition", "references", "crossrefs", {"tables"}) + " " + Latex::newAdditionalEntry(4, "grammar", "definition", "references", "crossrefs", {"tables"}) + " " + Latex::newAdditionalEntry(5, "grammar", "definition", "references", "crossrefs", {"tables"}) + " " + Latex::newAdditionalEntry(6, "grammar", "definition", "references", "crossrefs", {"tables"}) + " " + Latex::newPhrase("phrase", "definition", "example") + " " + Latex::newNote("note"));
        langComms.push_back(Latex::newPage());
        langComms.push_back(Latex::beginMulticols(2));

        GenericArray<false, rapidjson::Value> dictionary = lang["dictionary"].GetArray();
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
            string aTemp = a.word;
            string bTemp = b.word;

            transform(aTemp.begin(), aTemp.end(), aTemp.begin(), [](char& ch)->int {
                return tolower(ch);
            });

            transform(bTemp.begin(), bTemp.end(), bTemp.begin(), [](char& ch)->int {
                return tolower(ch);
            });

            return aTemp < bTemp;
        });

        char csc = '\0';
        vector<pair<string, pair<string, string>>> englishData;

        for (vector<DictEntry>::iterator deIter = entries.begin(); deIter != entries.end(); deIter++)
        {
            if (tolower(deIter->word[0]) != csc)
            {
                langComms.push_back(Latex::newSubSubSection(string(1, toupper(deIter->word[0]))));
                csc = deIter->word[0];
            }

            langComms.push_back(Latex::newEntry(deIter->word, deIter->pronunciaton, deIter->entries[0].tos, deIter->entries[0].definition, deIter->entries[0].references, deIter->entries[0].crossrefs, deIter->entries[0].tables));
            englishData.push_back(pair<string, pair<string, string>>(deIter->entries[0].engTrans, pair<string, string>(deIter->word, deIter->entries[0].tos)));

            for (int i = 1; i < (int)deIter->entries.size(); i++)
            {
                langComms.push_back(Latex::newAdditionalEntry(i + 1, deIter->entries[i].tos, deIter->entries[i].definition, deIter->entries[i].references, deIter->entries[i].crossrefs, deIter->entries[i].tables));
                englishData.push_back(pair<string, pair<string, string>>(deIter->entries[i].engTrans, pair<string, string>(deIter->word, deIter->entries[i].tos)));
            }

            langComms.push_back("\n");
        }

        langComms.push_back(Latex::endMulticols());
        langComms.push_back(Latex::newPage());
        langComms.push_back(Latex::newSubSection("English-" + name + " Dictionary"));
        langComms.push_back(Latex::beginMulticols(2));
        csc = '\0';
        
        sort(englishData.begin(), englishData.end(), [](const pair<string, pair<string, string>>& a, const pair<string, pair<string, string>>& b)->bool {
            string aTemp = a.first;
            string bTemp = b.first;

            transform(aTemp.begin(), aTemp.end(), aTemp.begin(), [](char& ch)->int {
                return tolower(ch);
            });

            transform(bTemp.begin(), bTemp.end(), bTemp.begin(), [](char& ch)->int {
                return tolower(ch);
            });

            return aTemp < bTemp;
        });

        for (vector<pair<string, pair<string, string>>>::iterator edIter = englishData.begin(); edIter != englishData.end(); edIter++)
        {
            if (tolower(edIter->first[0]) != csc)
            {
                langComms.push_back(Latex::newSubSubSection(string(1, toupper(edIter->first[0]))));
                csc = edIter->first[0];
            }

            langComms.push_back(Latex::newEntry(edIter->first, "", edIter->second.second, edIter->second.first, "", "", {}) + "\n");
        }

        langComms.push_back(Latex::endMulticols());
        langComms.push_back(Latex::newPage());
        langComms.push_back(Latex::newSubSection("Grammar Tables"));
        
        GenericArray<false, rapidjson::Value> tables = lang["tables"].GetArray();
        int tableNum = 0;

        for (Value* table = tables.Begin(); table != tables.End(); ++table)
        {
            vector<vector<string>> gTableData;
            vector<bool> gHasHLines;
            GenericObject<false, rapidjson::Value> tableObj = table->GetObject();
            string tableName = tableObj["name"].GetString();
            string id = tableObj["id"].GetString();
            vector<string> rowLabels, colLabels;
            GenericArray<false, rapidjson::Value> tColLabels = tableObj["colLabels"].GetArray(), tRowLabels = tableObj["rowLabels"].GetArray(), rows = tableObj["rows"].GetArray();
            string colStyles = tableObj["colStyles"].GetString();
            colLabels.push_back("");

            for (Value* colLabel = tColLabels.Begin(); colLabel != tColLabels.End(); ++colLabel)
            {
                colLabels.push_back(colLabel->GetString());
            }

            gTableData.push_back(colLabels);
            gHasHLines.push_back(true);

            for (Value* rowLabel = tRowLabels.Begin(); rowLabel != tRowLabels.End(); ++rowLabel)
            {
                rowLabels.push_back(rowLabel->GetString());
            }

            int i = 0;

            for (Value* r = rows.Begin(); r != rows.End(); ++r)
            {
                vector<string> rowData;
                rowData.push_back(rowLabels[i++]);
                GenericArray<false, rapidjson::Value> row = r->GetArray();

                for (Value* rr = row.Begin(); rr != row.End(); ++rr)
                {
                    rowData.push_back(rr->GetString());
                }

                gTableData.push_back(rowData);
                gHasHLines.push_back(false);
            }

            langComms.push_back(Latex::makeTableWithLabel(gTableData, gHasHLines, "ht", colStyles, true, tableName, id));

            if (((tableNum + 1) % 3) == 0)
            {
                langComms.push_back(Latex::newPage());
            }

            tableNum += 1;
        }

        if (lang.HasMember("addAfter"))
        {
            langComms.push_back(Latex::newPage());
            GenericArray<false, rapidjson::Value> addAfter = lang["addAfter"].GetArray();

            for (Value* aaIter = addAfter.Begin(); aaIter != addAfter.End(); ++aaIter)
            {
                GenericObject<false, rapidjson::Value> afterObj = aaIter->GetObject();
                langComms.push_back(Latex::newSubSection(afterObj["sectionName"].GetString()));
                GenericArray<false, rapidjson::Value> sectionData = afterObj["sectionData"].GetArray();
                GenericObject<false, rapidjson::Value> englishData = sectionData[0].GetObject(), langData = sectionData[1].GetObject(), interData = sectionData[2].GetObject();
                langComms.push_back(Latex::newSubSubSection(englishData["subSectionName"].GetString()));
                GenericArray<false, rapidjson::Value> englishBody = englishData["sectionBody"].GetArray();

                for (Value* ebIter = englishBody.Begin(); ebIter != englishBody.End(); ++ebIter)
                {
                    langComms.push_back(ebIter->GetString());
                    langComms.push_back("\n");
                }

                langComms.push_back(Latex::newSubSubSection(langData["subSectionName"].GetString())); // Before this
                GenericArray<false, rapidjson::Value> langBody = langData["sectionBody"].GetArray();

                for (Value* lbIter = langBody.Begin(); lbIter != langBody.End(); ++lbIter)
                {
                    langComms.push_back(lbIter->GetString());
                    langComms.push_back("\n");
                }

                langComms.push_back(Latex::newSubSubSection(interData["subSectionName"].GetString()));
                GenericArray<false, rapidjson::Value> interBody = interData["interlinearData"].GetArray();

                for (Value* ibIter = interBody.Begin(); ibIter != interBody.End(); ++ibIter)
                {
                    GenericObject<false, rapidjson::Value> ibData = ibIter->GetObject();
                    langComms.push_back("\\ex");
                    langComms.push_back("\\begingl");
                    langComms.push_back("% " + string(ibData["transText"].GetString()));
                    langComms.push_back("\\gla " + string(ibData["langText"].GetString()) + "//");
                    langComms.push_back("\\glb " + string(ibData["englishText"].GetString()) + "//");
                    string pos = "\\glc ";
                    GenericArray<false, rapidjson::Value> posData = ibData["posText"].GetArray();
                    int i = 0;

                    for (Value* posdIter = posData.Begin(); posdIter != posData.End(); ++posdIter)
                    {
                        pos += "\\tiny{\\sc ";
                        pos += posdIter->GetString();
                        pos += "}";

                        if (i < ((int)posData.Size() - 1))
                        {
                            pos += " ";
                        }

                        i += 1;
                    }

                    langComms.push_back(pos + "//");
                    langComms.push_back("\\endgl");
                    langComms.push_back("\\xe");
                }
            }
        }

        commands.insert(commands.end(), langComms.begin(), langComms.end());
    }

    doc.setDocument(commands);
    return doc.toString();
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