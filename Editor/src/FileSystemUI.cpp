#include "FileSystemUI.h"

#include <src/Vendors/imgui/imgui.h>

#include <src/Vendors/imgui/imgui_internal.h>
#include "src/Resource/FontAwesome5.h"
#include "src/Scene/Components/ParentComponent.h"
#include "src/Rendering/Textures/Texture.h"
#include "src/Rendering/Textures/TextureManager.h"
#include "EditorInterface.h"

const std::string TEMPLATE_SCRIPT_BEGIN = "import \"Nuake:Engine\" for Engine \
import \"Nuake:ScriptableEntity\" for ScriptableEntity \
import \"Nuake:Input\" for Input \
import \"Nuake:Scene\" for Scene \
\
class ";

const std::string TEMPLATE_SCRIPT_END = " is ScriptableEntity {\
construct new(){\
	_ReloadSpeed = 0.1\
	_Intensity = 0.0\
}\
\
init() {\
}\
\
// Updates every frame\
update(ts) {\
\
}\
\
// Updates every tick\
fixedUpdate(ts) {\
	\
}\
\
exit() {\
}\
}";

namespace Nuake {
    // TODO: add filetree in same panel
    void FileSystemUI::Draw()
    {

    }

    void FileSystemUI::DrawDirectoryContent()
    {
    }

    void FileSystemUI::DrawFiletree()
    {
    }

    void FileSystemUI::EditorInterfaceDrawFiletree(Ref<Directory> dir)
    {
        ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
        //if (is_selected) 
       
        std::string icon = ICON_FA_FOLDER;
        if (m_CurrentDirectory == dir)
            base_flags |= ImGuiTreeNodeFlags_Selected;
        if (dir->Directories.size() <= 0)
            base_flags = ImGuiTreeNodeFlags_Leaf;

        bool open = ImGui::TreeNodeEx(dir->name.c_str(), base_flags);

        if (ImGui::IsItemClicked())
            m_CurrentDirectory = dir;

        if (open)
        {
            if (dir->Directories.size() > 0)
                for (auto& d : dir->Directories)
                    EditorInterfaceDrawFiletree(d);
            ImGui::TreePop();
        }
    }

    void FileSystemUI::DrawDirectory(Ref<Directory> directory)
    {
        ImGui::PushFont(EditorInterface::bigIconFont);
        std::string id = directory->name;

        if (ImGui::Button(id.c_str(), ImVec2(100, 100)))
            m_CurrentDirectory = directory;

        ImGui::Text(directory->name.c_str());
        ImGui::PopFont();
    }

    bool FileSystemUI::EntityContainsItself(Entity source, Entity target)
    {
        ParentComponent& targeParentComponent = target.GetComponent<ParentComponent>();
        if (!targeParentComponent.HasParent)
            return false;

        Entity currentParent = target.GetComponent<ParentComponent>().Parent;
        while (currentParent != source)
        {
            if (currentParent.GetComponent<ParentComponent>().HasParent)
                currentParent = currentParent.GetComponent<ParentComponent>().Parent;
            else
                return false;

            if (currentParent == source)
                return true;
        }
        return true;
    }

    void FileSystemUI::DrawFile(Ref<File> file)
    {
        ImGui::PushFont(EditorInterface::bigIconFont);
        if (file->Type == ".png" || file->Type == ".jpg")
        {
            Ref<Texture> texture = TextureManager::Get()->GetTexture(file->fullPath);
            ImGui::ImageButton((void*)texture->GetID(), ImVec2(100, 100), ImVec2(0, 1), ImVec2(1, 0));
        }
        else
        {
            const char* icon = ICON_FA_FILE;
            if (file->Type == ".shader")
                icon = ICON_FA_FILE_CODE;
            if (file->Type == ".map")
                icon = ICON_FA_BROOM;
            if (file->Type == ".ogg" || file->Type == ".mp3" || file->Type == ".wav" || file->Type == ".flac")
                icon = ICON_FA_FILE_AUDIO;
            if (file->Type == ".wren")
                icon = ICON_FA_FILE_CODE;
            if (file->Type == ".md3" || file->Type == ".obj")
                icon = ICON_FA_FILE_IMAGE;

            std::string fullName = icon + std::string("##") + file->fullPath;
            if (ImGui::Button(fullName.c_str(), ImVec2(100, 100)))
            {

            }

            if (ImGui::BeginDragDropSource())
            {
                char pathBuffer[256];
                std::strncpy(pathBuffer, file->fullPath.c_str(), sizeof(pathBuffer));
                std::string dragType;
                if (file->Type == ".wren")
                    dragType = "_Script";
                else if (file->Type == ".map")
                    dragType = "_Map";
                else if (file->Type == ".obj" || file->Type == ".mdl" || file->Type == ".gltf" || file->Type == ".md3" || file->Type == ".fbx")
                    dragType = "_Model";
                else if (file->Type == ".interface")
                    dragType = "_Interface";
                else if (file->Type == ".prefab")
                    dragType = "_Prefab";

                ImGui::SetDragDropPayload(dragType.c_str(), (void*)(pathBuffer), sizeof(pathBuffer));
                ImGui::Text(file->name.c_str());
                ImGui::EndDragDropSource();
            }
        }

        ImGui::Text(file->name.c_str());
        ImGui::PopFont();
    }

    bool Splitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2, float splitter_long_axis_size = -1.0f)
    {
        using namespace ImGui;
        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = g.CurrentWindow;
        ImGuiID id = window->GetID("##Splitter");
        ImRect bb;
        bb.Min = window->DC.CursorPos + (split_vertically ? ImVec2(*size1, 0.0f) : ImVec2(0.0f, *size1));
        bb.Max = bb.Min + CalcItemSize(split_vertically ? ImVec2(thickness, splitter_long_axis_size) : ImVec2(splitter_long_axis_size, thickness), 0.0f, 0.0f);
        return SplitterBehavior(bb, id, split_vertically ? ImGuiAxis_X : ImGuiAxis_Y, size1, size2, min_size1, min_size2, 0.0f);
    }

    float h = 200;
    static float sz1 = 300;
    static float sz2 = 300;
    void FileSystemUI::DrawDirectoryExplorer()
    {
        if (ImGui::Begin("File browser"))
        {
            Ref<Directory> rootDirectory = FileSystem::GetFileTree();
            if (!rootDirectory)
                return;

            ImVec2 avail = ImGui::GetContentRegionAvail();
            Splitter(true, 4.0f, &sz1, &sz2, 100, 8, avail.y);

            if (ImGui::BeginChild("Tree browser", ImVec2(sz1, avail.y)))
            {
                ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_SpanAvailWidth;

                bool is_selected = m_CurrentDirectory == FileSystem::RootDirectory;
                if (is_selected)
                    base_flags |= ImGuiTreeNodeFlags_Selected;

                std::string icon = ICON_FA_FOLDER;
                bool open = ImGui::TreeNodeEx((icon + " " + "Project files").c_str(), base_flags);
                if (ImGui::IsItemClicked())
                {
                    m_CurrentDirectory = FileSystem::RootDirectory;
                }

                if (open)
                {
                    for(auto& d : rootDirectory->Directories)
                        EditorInterfaceDrawFiletree(d);

                    ImGui::TreePop();
                }

            }
            ImGui::EndChild();
            ImGui::SameLine();

            avail = ImGui::GetContentRegionAvail();

            std::vector<Ref<Directory>> paths = std::vector<Ref<Directory>>();

            Ref<Directory> currentParent = m_CurrentDirectory;
            paths.push_back(m_CurrentDirectory);

            while (currentParent != nullptr) 
            {
                paths.push_back(currentParent);
                currentParent = currentParent->Parent;
            }


            avail = ImGui::GetContentRegionAvail();
            if (ImGui::BeginChild("Wrapper", avail))
            {
                avail.y = 30;
                if (ImGui::BeginChild("Path", avail))
                {
                    if (ImGui::Button("Refresh"))
                    {
                        FileSystem::Scan();
                        m_CurrentDirectory = FileSystem::RootDirectory;
                    }

                    ImGui::SameLine();
                    for (int i = paths.size() - 1; i > 0; i--) 
                    {
                        if (i != paths.size())
                            ImGui::SameLine();

                        if (ImGui::Button(paths[i]->name.c_str()))
                            m_CurrentDirectory = paths[i];

                        ImGui::SameLine();
                        ImGui::Text("/");
                    }

                    ImGui::EndChild();
                }

                avail = ImGui::GetContentRegionAvail();

                if (ImGui::BeginChild("Content", avail))
                {
                    int width = avail.x;
                    ImVec2 buttonSize = ImVec2(80, 80);
                    int amount = (int)(width / 200);
                    if (amount <= 0) amount = 1;

                    int i = 1; // current amount of item per row.
                    if (ImGui::BeginTable("ssss", amount))
                    {
                        // Button to go up a level.
                        if (m_CurrentDirectory && m_CurrentDirectory != FileSystem::RootDirectory && m_CurrentDirectory->Parent)
                        {
                            ImGui::TableNextColumn();
                            if (ImGui::Button("..", ImVec2(100, 100)))
                                m_CurrentDirectory = m_CurrentDirectory->Parent;
                            ImGui::TableNextColumn();
                            i++;
                        }

                        if (m_CurrentDirectory && m_CurrentDirectory->Directories.size() > 0)
                        {
                            for (Ref<Directory>& d : m_CurrentDirectory->Directories)
                            {
                                DrawDirectory(d);

                                if (i + 1 % amount != 0)
                                    ImGui::TableNextColumn();
                                else
                                    ImGui::TableNextRow();

                                i++;
                            }
                        }

                        if (m_CurrentDirectory && m_CurrentDirectory->Files.size() > 0)
                        {
                            for (auto f : m_CurrentDirectory->Files)
                            {
                                DrawFile(f);
                                if (i - 1 % amount != 0)
                                    ImGui::TableNextColumn();
                                else
                                    ImGui::TableNextRow();
                                i++;
                            }
                        }
						

		

                        if (ImGui::BeginPopupContextWindow())
                        {
                            if (ImGui::MenuItem("New Wren script"))
                            {
                                ImGui::OpenPopup("new_file");

                            }

                            if (ImGui::MenuItem("New interface script"))
                            {
                            }

                            if (ImGui::MenuItem("New Scene"))
                            {
                            }

                            if (ImGui::MenuItem("New folder"))
                            {
                            }

                            if (ImGui::MenuItem("New interface"))
                            {
                            }

                            if (ImGui::MenuItem("New stylesheet"))
                            {
                            }

							ImGui::EndPopup();
                        }

						if (ImGui::BeginPopup("new_file"))
						{
							static char name[32] = "NewWrenScript";
							char buf[64];
							sprintf(buf, "Button: %s###Button", name); // ### operator override ID ignoring the preceding label

							ImGui::Text("Edit name:");
							ImGui::InputText("##edit", name, IM_ARRAYSIZE(name));
							if (ImGui::Button("Close"))
								ImGui::CloseCurrentPopup();
							if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)) && name != "")
							{
								FileSystem::BeginWriteFile(m_CurrentDirectory->fullPath + name + ".wren");

								FileSystem::WriteLine(TEMPLATE_SCRIPT_BEGIN + name + TEMPLATE_SCRIPT_END);

								FileSystem::EndWriteFile();
								ImGui::CloseCurrentPopup();
							}

							ImGui::EndPopup();
						}
                        ImGui::EndTable();
                    }

                }
                ImGui::EndChild();
            }
            ImGui::EndChild();
        }
        ImGui::End();
    }
}
