import zipfile, os, re, html
from pathlib import Path
OUT=Path('Final_Deliverables'); OUT.mkdir(exist_ok=True)
def esc(s): return html.escape(s,quote=False)
def para(text,style='Normal',page=False):
    br='<w:r><w:br w:type="page"/></w:r>' if page else ''
    return f'<w:p><w:pPr><w:pStyle w:val="{style}"/></w:pPr>{br}<w:r><w:t xml:space="preserve">{esc(text)}</w:t></w:r></w:p>'
cover=['RP KARONGI COLLEGE','DEPARTMENT OF ICT','PROGRAM: INFORMATION TECHNOLOGY (IT)','','CAT PRACTICE: PROJECT BASED','','MOTHERLAND PHARMACY MANAGEMENT SYSTEM','','COURSE: DATA STRUCTURES AND ALGORITHMS','CASE STUDY: MOTHERLAND PHARMACY','','PREPARED BY:','THIERRY NDAYISHIMIYE','','INSTITUTION: RP KARONGI COLLEGE','DATE: 1 August 2026']
body=[para(x,'Title' if i==6 else 'Subtitle' if i in (0,1,2,4,8,9,11,12,14,15) else 'Normal') for i,x in enumerate(cover)]
# End the cover as its own section. The first section has no footer; the
# remaining document inherits the final section footer with page numbers.
body.append('<w:p><w:pPr><w:sectPr><w:type w:val="nextPage"/></w:sectPr></w:pPr></w:p>')
files=['01_Project_Report.md','02_Algorithms.md','03_Pseudocode.md','04_Flowcharts.md','06_ER_Diagram.md','07_Database_Documentation.md','08_Module_Documentation.md','13_Testing_Documentation.md','14_Screenshots_Guide.md','20_Build_Instructions.md']
for fn in files:
    for line in (Path('Project_Documentation')/fn).read_text(encoding='utf8').splitlines():
        if not line.strip(): continue
        if line.startswith('# '): body.append(para(line[2:],'Heading1'))
        elif line.startswith('## '): body.append(para(line[3:],'Heading2'))
        elif line.startswith('### '): body.append(para(line[4:],'Heading3'))
        elif line.startswith('```'): continue
        elif line.startswith('|'): body.append(para(line.replace('|','  ').strip(),'Normal'))
        elif line.startswith('- '): body.append(para('• '+line[2:],'Normal'))
        else: body.append(para(re.sub(r'`','',line),'Normal'))
docxml='''<?xml version="1.0" encoding="UTF-8" standalone="yes"?><w:document xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main"><w:body>'''+''.join(body)+'''<w:sectPr><w:pgSz w:w="12240" w:h="15840"/><w:pgMar w:top="1440" w:right="1440" w:bottom="1440" w:left="1440"/><w:footerReference w:type="default" r:id="rId1" xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"/></w:sectPr></w:body></w:document>'''
styles='''<?xml version="1.0" encoding="UTF-8"?><w:styles xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main"><w:docDefaults><w:rPrDefault><w:rPr><w:rFonts w:ascii="Times New Roman" w:hAnsi="Times New Roman"/><w:sz w:val="24"/></w:rPr></w:rPrDefault></w:docDefaults><w:style w:type="paragraph" w:default="1" w:styleId="Normal"><w:name w:val="Normal"/><w:pPr><w:spacing w:line="360" w:lineRule="auto" w:after="120"/></w:pPr></w:style><w:style w:type="paragraph" w:styleId="Title"><w:name w:val="Title"/><w:rPr><w:b/><w:sz w:val="36"/></w:rPr><w:pPr><w:jc w:val="center"/><w:spacing w:after="180"/></w:pPr></w:style><w:style w:type="paragraph" w:styleId="Subtitle"><w:name w:val="Subtitle"/><w:rPr><w:b/><w:sz w:val="28"/></w:rPr><w:pPr><w:jc w:val="center"/></w:pPr></w:style><w:style w:type="paragraph" w:styleId="Heading1"><w:name w:val="Heading 1"/><w:rPr><w:b/><w:sz w:val="30"/></w:rPr><w:pPr><w:spacing w:before="240" w:after="120"/></w:pPr></w:style><w:style w:type="paragraph" w:styleId="Heading2"><w:name w:val="Heading 2"/><w:rPr><w:b/><w:sz w:val="26"/></w:rPr></w:style><w:style w:type="paragraph" w:styleId="Heading3"><w:name w:val="Heading 3"/><w:rPr><w:b/></w:rPr></w:style></w:styles>'''
footer='''<?xml version="1.0" encoding="UTF-8"?><w:ftr xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main"><w:p><w:pPr><w:jc w:val="right"/></w:pPr><w:r><w:t>Motherland Pharmacy Management System | Page </w:t></w:r><w:fldSimple w:instr="PAGE"/></w:p></w:ftr>'''
ct='''<?xml version="1.0" encoding="UTF-8"?><Types xmlns="http://schemas.openxmlformats.org/package/2006/content-types"><Default Extension="rels" ContentType="application/vnd.openxmlformats-package.relationships+xml"/><Default Extension="xml" ContentType="application/xml"/><Override PartName="/word/document.xml" ContentType="application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml"/><Override PartName="/word/styles.xml" ContentType="application/vnd.openxmlformats-officedocument.wordprocessingml.styles+xml"/><Override PartName="/word/footer1.xml" ContentType="application/vnd.openxmlformats-officedocument.wordprocessingml.footer+xml"/></Types>'''
rels='''<?xml version="1.0"?><Relationships xmlns="http://schemas.openxmlformats.org/package/2006/relationships"><Relationship Id="rId1" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships" Target="word/document.xml"/></Relationships>'''
dre='''<?xml version="1.0"?><Relationships xmlns="http://schemas.openxmlformats.org/package/2006/relationships"><Relationship Id="rId1" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles" Target="styles.xml"/><Relationship Id="rId2" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/footer" Target="footer1.xml"/></Relationships>'''
with zipfile.ZipFile(OUT/'Motherland_Pharmacy_Management_System_CAT_Report.docx','w',zipfile.ZIP_DEFLATED) as z:
    for n,x in {'[Content_Types].xml':ct,'_rels/.rels':rels,'word/document.xml':docxml,'word/styles.xml':styles,'word/footer1.xml':footer,'word/_rels/document.xml.rels':dre}.items(): z.writestr(n,x)

# Minimal editable PPTX
slides=[
('Motherland Pharmacy Management System',['RP Karongi College | Department of ICT','Program: Information Technology (IT)','Course: Data Structures and Algorithms','Student: Thierry NDAYISHIMIYE','Case Study: Motherland Pharmacy']),
('Project Description',['C11 console application connected to MySQL/MariaDB','Supports pharmacy records, stock-aware sales and reporting','Used by authenticated pharmacy staff','Built/tested in VS Code; configured for Dev-C++ 6.3']),
('Problem Statement',['Manual records can delay retrieval and reporting','Stock and expiry monitoring need consistency','Sales, suppliers and customer records need central storage']),
('Objectives',['Centralize pharmacy data in MySQL','Manage medicines, users, suppliers and customers','Record multi-item sales with stock updates','Block expired medicines and provide reports']),
('System Requirements',['Functional: login, CRUD, cart sales, reports, dashboard','Non-functional: reliability, usability, data integrity','Local XAMPP MySQL/MariaDB service is required']),
('Actual System Modules',['Main/menu and database connection','Login and session display','Users, suppliers, customers, medicines','Sales cart and receipts','Reports, dashboard and safe expiry cleanup']),
('System Workflow / Algorithm',['Start -> connectDB -> Main Menu -> Login','Valid login -> Dashboard -> selected module -> Dashboard','Sale: validate customer, stock and expiry; transaction commit/rollback','Logout -> Main Menu; Exit -> disconnectDB']),
('Database Design (ERD)',['users 1..* sales; customers 1..* sales','suppliers 1..* medicines','sales 1..* sale_items','medicines 1..* sale_items','Six actual tables: users, customers, suppliers, medicines, sales, sale_items']),
('System Implementation',['Language: C11','Database: pharmacy_db in XAMPP MySQL/MariaDB','Connector: MySQL Connector/C 6.1','Development: VS Code and Embarcadero Dev-C++ 6.3','Architecture: console modules -> Connector/C -> database']),
('Actual Screens to Demonstrate',['Login and Motherland Pharmacy dashboard','Medicine, supplier, customer and user menus','Sale cart and receipt','Expired medicine rejection','Reports, Smart Pharmacy Advice and System Overview']),
('Key Features',['Validation for contact, date, quantity, price, role and fields','Sell price cannot be below buy price','Expired medicines cannot be sold','Transactions preserve sale/stock consistency','Safe cleanup protects medicine sale history']),
('Conclusion',['The project delivers a local pharmacy management workflow','C implements menus, validation, cart array and algorithms','MySQL stores related operational records','Reports support stock and sales decisions']),
('Recommendations',['Future: GUI, password hashing, prepared statements','Role-based access control and audit logs','Barcode scanning, purchase/batch records, backups','Cloud database and advanced exports']),
('THANK YOU',['Questions and Answers','Motherland Pharmacy Management System'])]
def sp_xml(title,items):
    def box(x,y,w,h,text,size,bold=False): return f'<p:sp><p:nvSpPr><p:cNvPr id="1" name="Text"/><p:cNvSpPr/><p:nvPr/></p:nvSpPr><p:spPr><a:xfrm><a:off x="{x}" y="{y}"/><a:ext cx="{w}" cy="{h}"/></a:xfrm></p:spPr><p:txBody><a:bodyPr/><a:lstStyle/><a:p><a:r><a:rPr lang="en-US" sz="{size}" b="{1 if bold else 0}"/><a:t>{esc(text)}</a:t></a:r></a:p></p:txBody></p:sp>'
    shapes=box(600000,400000,11000000,900000,title,3200,True)
    for i,it in enumerate(items): shapes+=box(900000,1700000+i*430000,10000000,350000,'• '+it,1800,False)
    return f'''<?xml version="1.0" encoding="UTF-8"?><p:sld xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main" xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships" xmlns:p="http://schemas.openxmlformats.org/presentationml/2006/main"><p:cSld><p:spTree><p:nvGrpSpPr><p:cNvPr id="0" name=""/><p:cNvGrpSpPr/><p:nvPr/></p:nvGrpSpPr><p:grpSpPr/>{shapes}</p:spTree></p:cSld><p:clrMapOvr><a:masterClrMapping/></p:clrMapOvr></p:sld>'''
ct2='''<?xml version="1.0"?><Types xmlns="http://schemas.openxmlformats.org/package/2006/content-types"><Default Extension="rels" ContentType="application/vnd.openxmlformats-package.relationships+xml"/><Default Extension="xml" ContentType="application/xml"/><Override PartName="/ppt/presentation.xml" ContentType="application/vnd.openxmlformats-officedocument.presentationml.presentation.main+xml"/>'''+''.join(f'<Override PartName="/ppt/slides/slide{i}.xml" ContentType="application/vnd.openxmlformats-officedocument.presentationml.slide+xml"/>' for i in range(1,15))+'''</Types>'''
pres='''<?xml version="1.0"?><p:presentation xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main" xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships" xmlns:p="http://schemas.openxmlformats.org/presentationml/2006/main"><p:sldIdLst>'''+''.join(f'<p:sldId id="{255+i}" r:id="rId{i}"/>' for i in range(1,15))+'''</p:sldIdLst><p:sldSz cx="12192000" cy="6858000" type="screen16x9"/><p:notesSz cx="6858000" cy="9144000"/></p:presentation>'''
prels='''<?xml version="1.0"?><Relationships xmlns="http://schemas.openxmlformats.org/package/2006/relationships">'''+''.join(f'<Relationship Id="rId{i}" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/slide" Target="slides/slide{i}.xml"/>' for i in range(1,15))+'''</Relationships>'''
with zipfile.ZipFile(OUT/'Motherland_Pharmacy_Management_System_Presentation.pptx','w',zipfile.ZIP_DEFLATED) as z:
    z.writestr('[Content_Types].xml',ct2); z.writestr('_rels/.rels','<?xml version="1.0"?><Relationships xmlns="http://schemas.openxmlformats.org/package/2006/relationships"><Relationship Id="rId1" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument" Target="ppt/presentation.xml"/></Relationships>'); z.writestr('ppt/presentation.xml',pres); z.writestr('ppt/_rels/presentation.xml.rels',prels)
    for i,(t,items) in enumerate(slides,1): z.writestr(f'ppt/slides/slide{i}.xml',sp_xml(t,items)); z.writestr(f'ppt/slides/_rels/slide{i}.xml.rels','<?xml version="1.0"?><Relationships xmlns="http://schemas.openxmlformats.org/package/2006/relationships"/>')
