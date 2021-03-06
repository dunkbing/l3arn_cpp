#ifndef VCPROJECT_H_INCLUDED
#define VCPROJECT_H_INCLUDED

#include <wx/wx.h>
#include <wx/string.h>
#include <wx/xml/xml.h>
#include <wx/textfile.h>
#include <vector>

#include "CompileThread.h"

#include "main.h"

class VcSln;


struct CustomUnityBuild
{
    wxString m_name;
    wxArrayString m_files;
    MapStringToInt m_recursive;
    MapStringToString m_rule;
};
WX_DECLARE_OBJARRAY(CustomUnityBuild*,  ArrayOfCustomUnityBuild);



class VcProject
{
	//this it is intended to be used for precompiled headers.
	//So, i want to know what are the common headers to put them in a precompiled header
	//MapLongToString m_commonHeaders;

    /// m_includedInSolution specify if this VisualStudio project (vcproj) it is included in gcc solution.
    /// in other words this flag specify if this project will be compiled with gcc!
    bool m_includedInSolution;

    wxString m_name;

    /**
     * This is the file name to the VisualStudio project (*.vcproj) .
     */
    wxString m_fileName;
    

    bool m_targetChanged;


    ///call the external programs:
    static CommandDescriptorArray s_procLocalArray;
    static CommandDescriptorArray s_procDistccArray;


    int m_currentCUBIndex;
    int m_currentCUBFilterIsRecursive;
    wxString m_currentCUBFilterRule;

    //static void Abort(CommandDescriptorArray& procArray);

public:

    static CommandDescriptorArray s_linkCommandList;
    static CommandDescriptorArray s_compileCommandList;

    Declarations m_projectDecl;

    /** wxString <OutDir> from VS project file*/
    wxString m_targetOutDirFromVS;

    /** wxString <IntDir> from VS project file*/
    wxString m_targetIntDirFromVS;

    /** wxString <TargetName> from VS project file*/
    wxString m_targetNameFromVS;

    /** wxString <TargetExt> from VS project file*/
    wxString m_targetOutTypeFromVS;

	wxString m_targetOutType;

    /**
     * This is the file with project settings.
     * Is an xml file that will have only the <Project> </Project> node similar with the 
     * Project node from sln2gcc.xml. All settings will be loaded first from s2g file and then 
     * overwited by the settings from sln2gcc.xml
     */
    wxString m_s2gFile;

    ///a wxArrayString with extensions.
    wxArrayString m_typesOfFilesToBeCompiled;

    wxArrayString m_headerFiles;
    //bool m_usePCH;
    wxArrayString m_sourceFiles;
    wxArrayString m_filesIgnored;
    wxArrayString m_filtersIgnored;
    MapStringToString m_fileSpecificCFLAGS;

    bool m_useAutoGeneratedUnityBuilds;
    ArrayOfCustomUnityBuild m_customUBFiles;
    wxArrayString m_allUBFiles;
    long m_autoGeneratedUBNumber;
    wxArrayString m_excludeFileFromAutoGenUB;

    /**
     * This is the real name of the Visual Studio configuration  that match the request . For example in in sln2gcc.xml (or in *.s2g )  
     * file you have <MSVCConfiguration Debug="Debug" Release="Release"/>  the m_MSVCConfigurationUsed will have value "Debug|Win32" or "Release|Win32"
     */
    wxString m_MSVCConfigurationUsed;

	bool m_useAlreadyCompiledLib;
	static bool s_atLeastOneCompiledLibUsed;
	static wxArrayString s_newlyCompiledLibs;

    VcProject(void);
    VcProject(const wxString& name, const wxString& fileName);
    virtual ~VcProject(void);
    static void Clear(void);
    wxString GetFileName();
    wxString GetWrokingPath();
    void SetFileName(const wxString& fileName);
    wxString GetName();
    void SetName(const wxString& name);

    bool IsIncludedInProject();
    void SetIncludedInSolution(bool includedInProject = true);

    void SetProjectCS(const Declarations& projectDecl);

    bool Parse();
    bool ParseVCPROJ();
    bool ParseVCXPROJ();
    void CloseAndRenameCurrentAUB(wxFile*& unityFile, wxString unityFileName, wxString unityFileNameTemp, bool alreadyExists);
    bool PrepareCompileCommands(Declarations decl, VcSln *sln, bool forceRelink = false, bool forceRecompile = false, bool isMainProject = false);
    static bool Compile();
    //static void AbortAll();
    static bool Link();
	static void CopyLibs();

    void CollectVcprojFiles(wxXmlNode* filesNode);
    void CollectVcxprojFiles(wxXmlNode* node);
    bool IsSourceFile(wxString extension);
    bool IsHeaderFile(wxString extension);

    bool HasTargetChanged();

    ///RemoveEndedProcessesFromQueue
    ///procArray the array with processes
    ///processesLeft maximum processes that can be in the vector
    static bool RemoveEndedProcessesFromQueue(CommandDescriptorArray& procArray , unsigned int processesLeft, bool convertErrorsToWarnings = false);

    void RemoveFilesFromCompilingQueue(wxString UnityBuildFile, CommandDescriptorArray& compileCommands, LinkCommandDescriptor& linkCommands);

    /**
     * IsExcludedFromUnityBuild test if a file can be included in Unity Build or not.
     */
    bool IsExcludedFromUnityBuild(wxString fileName);

    /**
     * IsIgnored test if a file if ignored or not.
     * If a file is ignored means taht teat file will not be compiled!
     */
    bool IsIgnored(wxString fileName);


    /**
     * FileNameMatch checks if a fileName matches the rule containing * and/or ? (ex: *.ext or ???.ext)
     */
    bool FileNameMatch(wxString fileName, wxString regex);


    bool CompareFiles(wxString file1, wxString file2);
    bool CompareFiles(wxTextFile& file1, wxTextFile& file2);
};

#endif //VCPROJECT_H_INCLUDED
