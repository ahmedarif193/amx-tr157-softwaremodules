include ../makefile.inc

DOC_DIR=../output/doc/
DOC_TITLE="TR181 Logical Interface Manager Data Model"
DOC_SUB_TITLE="LogicalInterface"

doc: xml html confluence

xml:
	mkdir -p $(DOC_DIR)/xml
	amxo-cg -Gxml,$(DOC_DIR)/xml ../odl/tr181-logical.odl

html: xml
	mkdir -p $(DOC_DIR)/html
	amxo-xml-to -x html \
				-o output-dir=$(DOC_DIR)/html/ \
				-o title=$(DOC_TITLE) \
				-o version=$(VERSION) \
				-o sub-titel=$(DOC_SUB_TITLE) \
				$(DOC_DIR)/xml/*.xml

confluence: xml
	mkdir -p $(DOC_DIR)/confluence
	amxo-xml-to -x confluence \
				-o output-dir=$(DOC_DIR)/confluence \
				-o title=$(DOC_TITLE) \
				-o version=$(VERSION) \
				-o sub-title=$(DOC_SUB_TITLE) \
				$(DOC_DIR)/xml/*.xml

clean:
	rm -rf $(DOC_DIR)

.PHONY: xml html confluence doc clean
