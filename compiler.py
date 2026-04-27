import xml.etree.ElementTree as ET
import struct
import unicodedata

def clean_and_uppercase(text):
    # Normalize to separate letters from their accent marks
    nfd_form = unicodedata.normalize('NFD', text)
    # Drop the accents
    no_accents = ''.join([c for c in nfd_form if unicodedata.category(c) != 'Mn'])
    # Force everything to UPPERCASE
    return no_accents.upper()

# CONFIG
XML_FILE = "./data/raw/fra-eng.tei"
OUTPUT_BIN = "./data/compiled/fra-eng.db"
ENTRY_FORMAT = "32s96s" # 32 bytes French, 96 bytes English

print("Parsing XML, stripping accents, and capitalizing...")
tree = ET.parse(XML_FILE)
root = tree.getroot()

entries = []
ns = "{http://www.tei-c.org/ns/1.0}"

for entry in root.findall(f'.//{ns}entry'):
    orth = entry.find(f'.//{ns}orth')
    
    if orth is not None and orth.text:
        # Clean and uppercase the French word
        fr = clean_and_uppercase(orth.text.strip())
        
        quotes = entry.findall(f'.//{ns}quote')
        
        for quote in quotes:
            if quote.text:
                # Uppercase the English translation as well
                en = quote.text.strip().upper()
                entries.append((fr.encode('utf-8')[:31], en.encode('utf-8')[:95]))

print("Sorting binary records (Pure ASCII Uppercase)...")
entries.sort()

print("Writing to disk...")
with open(OUTPUT_BIN, 'wb') as f:
    for fr, en in entries:
        f.write(struct.pack(ENTRY_FORMAT, fr, en))

print(f"SUCCESS: Wardwordward.db created with {len(entries)} entries. ALL CAPS.")
